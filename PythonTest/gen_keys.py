# -*- encoding: utf-8 -*-
import binascii
from ecdsa import SigningKey
from ecdsa.curves import SECP256k1


# 生成签名钥（私钥）
signning_key = SigningKey.generate(curve=SECP256k1)
sk = binascii.hexlify(signning_key.to_string()).decode('utf-8')
# 此时的私钥是十六进制格式
# 16进制:f97c89aaacf0cd2e47ddbacc97dae1f88bec49106ac37716c451dcdd008a4b62
import hashlib, base58
# alias method
decode_hex = binascii.unhexlify

# wallet import format key - base58 encoded format
def gen_wif_key(private_key):
    # prepended mainnet version byte to private key
    mainnet_private_key = '80' + private_key

    # perform SHA-256 hash on the mainnet_private_key
    sha256 = hashlib.sha256()
    sha256.update( decode_hex(mainnet_private_key) )
    hash = sha256.hexdigest()

    # perform SHA-256 on the previous SHA-256 hash
    sha256 = hashlib.sha256()
    sha256.update( decode_hex(hash) )
    hash = sha256.hexdigest()

    # create a checksum using the first 4 bytes of the previous SHA-256 hash
    # append the 4 checksum bytes to the mainnet_private_key
    checksum = hash[:8]
    hash = mainnet_private_key + checksum

    # convert mainnet_private_key + checksum into base58 encoded string
    return base58.b58encode( decode_hex(hash) )

# 生成wif 格式私钥：
# 如'5JQnzf94d3Ys2vZxUp23XmzNVUBqiQxXDZMG9sW9dyVyuewu2q9'

wif_key = gen_wif_key(sk).decode()
# --------------------------
from bitcoin.wallet import CBitcoinSecret, P2PKHBitcoinAddress

secret = CBitcoinSecret(wif_key)
# 生成地址
address = P2PKHBitcoinAddress.from_pubkey(secret.pub) 

print("Privkey is {0}".format(wif_key)) # 打印私钥
print("Pubkey is {0}".format(secret.pub.hex())) # 打印公钥
print('Address is {0}'.format(address)) # 打印地址