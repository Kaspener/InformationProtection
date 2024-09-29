import random
from enum import Enum
from hashlib import sha3_512
from math import ceil
from typing import Tuple
from sys import byteorder

class Vote(Enum):
    YES = 1
    NO = 2
    ABSTRAIN = 3

def modul_pow(basis: int, indicator: int, modul: int) -> int:
    if basis < 0:
        raise ValueError("Basis must be a natural number!")
    if indicator < 0:
        raise ValueError("Indicator must be a natural number!")
    if modul <= 0:
        raise ValueError("Modul must be greater than zero!")

    result = 1
    basis %= modul

    while indicator > 0:
        if indicator & 1:
            result = (result * basis) % modul
        indicator >>= 1
        basis = (basis * basis) % modul

    return result

def euclid(a: int, b: int) -> Tuple[int, int, int]:

    if a <= 0 or b <= 0:
        raise ValueError("Numbers must be natural")
    if a > b:
        a, b = b, a
    u = [a, 1, 0]
    v = [b, 0, 1]
    while v[0] != 0:
        q = u[0] // v[0]
        t = [u[0] % v[0], u[1] - q * v[1], u[2] - q * v[2]]
        u, v = v, t
    return u

def inverse(n, p):
    gcd, inv, _ = euclid(n, p)
    assert gcd == 1
    if inv < 0 :
        inv += p
    return inv

def miiller_test(d: int, n: int) -> bool:
    a = 2 + random.randint(0, n - 4)

    if d < 0:
        return False

    x = modul_pow(a, d, n)

    if x == 1 or x == n - 1:
        return True

    while d != n - 1:
        x = (x * x) % n
        d *= 2

        if x == 1:
            return False
        if x == n - 1:
            return True

    return False


def is_prime(number: int, accuracy: int = 10) -> bool:
    if number <= 1 or number == 4:
        return False
    if number <= 3:
        return True
    if number % 2 == 0:
        return False

    d = number - 1
    while d % 2 == 0:
        d //= 2

    for _ in range(accuracy):
        if not miiller_test(d, number):
            return False

    return True


def gen_prime(min_value: int, max_value: int) -> int:
    while True:
        Q = random.randint(min_value, max_value)
        if is_prime(Q):
            return Q
        
def gen_mutually_prime(P: int) -> int:
    if P <= 2:
        raise ValueError("P must be greater than 2")

    while True:
        number = random.randint(2, P - 2)
        if euclid(number, P)[0] == 1:
            return number
        
class Server:
    def __init__(self):
        P = gen_prime((1<<1023), (1<<1024)-1)
        Q = gen_prime((1<<1023), (1<<1024)-1)
        while P==Q:
            Q = gen_prime((1<<1023), (1<<1024)-1)
        self.N = P*Q
        fi = (P-1)*(Q-1)
        self.d = gen_mutually_prime(fi)
        self._c = inverse(self.d, fi)
        self._voted = set()
        self.votes = list()

    def get_blank(self, client_name: str, hash: int) -> int:
        print(f"Request a blank from {client_name}")
        if client_name in self._voted:
            print(f"{client_name} has already voted")
            return None
        print(f"The blank was sent to {client_name}")
        self._voted.add(client_name)
        return modul_pow(hash, self._c, self.N)
    
    def set_blank(self, n: int, s: int) -> bool:
        hash = int(sha3_512(n.to_bytes(ceil(n.bit_length() / 8), byteorder=byteorder)).hexdigest(), 16)
        if (hash == modul_pow(s, self.d, self.N)):
            self.votes.append((n, s))
            return True
        else:
            print("Blank was rejected")
            return False
        
    def calculate_results(self):
        votes = {i.value: 0 for i in Vote}
        for n, s in self.votes:
            votes[n&3]+=1
        print("Results:")
        print(f"YES = {votes[1]}")
        print(f"NO = {votes[2]}")
        print(f"ABSTRAIN = {votes[3]}")

class Client:
    def __init__(self, server: Server, name: str):
        self.name = name
        self.server = server
        
    def vote(self, vote: Vote):
        rnd = gen_prime((1<<511), (1<<512)-1)
        n = rnd << 512 | vote.value
        r = gen_mutually_prime(self.server.N)
        hash = int(sha3_512(n.to_bytes(ceil(n.bit_length() / 8), byteorder=byteorder)).hexdigest(), 16)
        h = hash * modul_pow(r, self.server.d, self.server.N)
        s_ = self.server.get_blank(self.name, h)
        if s_:
            s = (s_*inverse(r, self.server.N))%self.server.N
            self.server.set_blank(n, s)

def main():
    server = Server()

    client = Client(server, "Aboba")
    client.vote(Vote.YES)

    client = Client(server, "Boba")
    client.vote(Vote.YES)

    client = Client(server, "Koba")
    client.vote(Vote.NO)

    client = Client(server, "Mova")
    client.vote(Vote.YES)

    client = Client(server, "Arbuz")
    client.vote(Vote.ABSTRAIN)

    client = Client(server, "Arbuz")
    client.vote(Vote.YES)

    server.calculate_results()

if __name__ == "__main__":
    main()