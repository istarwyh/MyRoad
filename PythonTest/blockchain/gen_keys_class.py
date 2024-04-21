# -*- encoding: utf-8 -*-
import binascii
import base58
import hashlib
from ecdsa import SigningKey
from ecdsa.curves import SECP256k1
from bitcoin.wallet import CBitcoinSecret, P2PKHBitcoinAddress

# 原本私钥是十六进制格式:
#f97c89aaacf0cd2e47ddbacc97dae1f88bec49106ac37716c451dcdd008a4b62
class Gen_Keys:

    def __init__(self):
        # alias method
        self.__decode_hex = binascii.unhexlify
        # 生成签名钥（私钥）
        self. __private_key = binascii.hexlify(
            # SECP256k1 椭圆曲线算法
            SigningKey.generate(curve=SECP256k1).to_string()).decode
    # wallet import format key - base58 encoded format
    def gen_wif_key(self, private_key):
        mainnet_private_key = '80' + private_key
        # perform SHA-256 hash on the mainnet_private_key
        sha256 = hashlib.sha256()
        sha256.update(self.__decode_hex(mainnet_private_key))
        hash = sha256.hexdigest()
        # perform SHA-256 on the previous SHA-256 hash
        sha256 = hashlib.sha256()
        sha256.update(self.__decode_hex(hash))
        hash = sha256.hexdigest()
        # create a checksum using the first 4 bytes of the previous SHA-256 hash
        # append the 4 checksum bytes to the mainnet_private_key
        checksum = hash[:8]
        hash = mainnet_private_key + checksum
        # convert mainnet_private_key + checksum into base58 encoded string
        return base58.b58encode(self.__decode_hex(hash)).decode()
    # 生成wif 格式私钥：
    # 如'5JQnzf94d3Ys2vZxUp23XmzNVUBqiQxXDZMG9sW9dyVyuewu2q9'
    def getWifKey(self):
        return gen_wif_key(self.__private_key).decode()
    #对私钥base58格式的封装
    def getSecret(self, wif_key):
        return CBitcoinSecret(wif_key)
    # 生成地址
    def getAddress(self, secret):
        return P2PKHBitcoinAddress.from_pubkey(secret.pub)

    def print_keys(self):
        wif_key = self.getWifKey()
        secret = self.getSecret(wif_key)
        address = self.getAddress(secret)
        # 打印私钥
        print("Privkey is {0}".format(wif_key))
        # 打印公钥
        print("Pubkey is {0}".format(secret.pub.hex()))
        # 打印地址
        print('Address is {0}'.format(address))
