import hashlib
import json
from time import time
# Blockchain 类负责管理链式数据，它会存储交易，还有添加新的区块到已有的区块链上。


class Blockchain(object):
    def __init__(self):
        self.chain = []
        self.current_transactions = []

    def new_block(self, proof, previous_hash=None):
        """
        创建一个新的区块到区块链中
        :param proof: <int> 由工作证明算法生成的证明
        :param previous_hash: (Optional) <str> 前一个区块的 hash 值
        :return: <dict> 新区块
        """

        block = {
            'index': len(self.chain) + 1,
            'timestamp': time(),
            'transactions': self.current_transactions,
            'proof': proof,
            'previous_hash': previous_hash,
        }

        # 重置当前交易记录
        self.current_transactions = []

        self.chain.append(block)
        return block

        # Adds a new transaction to the list of transactions
    def new_transaction(self, sender, recipient, amount):
        """
        Creates a new transaction to go into the next mined Block
        :param sender: <str> Address of the Sender
        :param recipient: <str> Address of the Recipient
        :param amount: <int> Amount
        :return: <int> The index of the Block that will hold this transaction
        """

        self.current_transactions.append({
            'sender': sender,
            'recipient': recipient,
            'amount': amount,
        })

        return self.last_block['index'] + 1

    @staticmethod
    def hash(block):
        """
        给一个区块生成 SHA-256 值
        :param block: <dict> Block
        :return: <str>
        """

        # 我们必须确保这个字典（区块）是经过排序的，否则我们将会得到不一致的散列
        block_string = json.dumps(block, sort_keys=True).encode()
        return hashlib.sha256(block_string).hexdigest()

    @property
    def last_block(self):
        return self.chain[-1]
    # PoW 的目标是计算出一个符合特定条件的数字，这个数字对于所有人而言必须在计算上非常困难，但易于验证。如 hash(x * y) = ac23dc...0。设 x = 5，求 y 。
    # from hashlib import sha256
    # x = 5
    # y = 0  # We don't know what y should be yet...
    # while sha256(f'{x*y}'.encode()).hexdigest()[-1] != "0":
    #     y += 1
    # print(f'The solution is y = {y}')
    def proof_of_work(self, last_nonce):
        """
        Simple Proof of Work Algorithm:
         - Find a number p' such that hash(pp') contains leading 4 zeroes, where p is the previous p'
         - p is the previous nonce, and p' is the new nonce
        :param last_nonce: <int>
        :return: <int>
        """
        # Number once,一个只被使用一次的任意或非重复的随机数值。
        nonce = 0
        while self.valid_proof(last_nonce, nonce) is False:
            nonce += 1

        return nonce

    @staticmethod
    def valid_proof(last_nonce, nonce):
        """
        Validates the Proof: Does hash(last_nonce, nonce) contain 4 leading zeroes?
        :param last_nonce: <int> Previous Proof
        :param nonce: <int> Current Proof
        :return: <bool> True if correct, False if not.
        """

        guess = f'{last_nonce}{nonce}'.encode()
        guess_hash = hashlib.sha256(guess).hexdigest()
        return guess_hash[:4] == "0000"

# block = {
#     'index': 1,
#     'timestamp': 1506057125.900785,
#     'transactions': [
#         {
#             'sender': "8527147fe1f5426f9dd545de4b27ee00",
#             'recipient': "a77f5cdfa2934df3954a5c7c7da5df1f",
#             'amount': 5,
#         }
#     ],
#     "hash":"00000000000000000018b0a6ae560fa33c469b6528bc9e0fb0c669319a186c33",
# 　　 "merkleroot":"5f8f8e053fd4c0c3175c10ac5189c15e6ba218909319850936fe54934dcbfeac",
#     'previous_hash': "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824"
# }
