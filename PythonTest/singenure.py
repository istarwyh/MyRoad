from bitcoin.wallet import CBitcoinSecret, P2PKHBitcoinAddress
from bitcoin.signmessage import BitcoinMessage, VerifyMessage, SignMessage

key = '5JQnzf94d3Ys2vZxUp23XmzNVUBqiQxXDZMG9sW9dyVyuewu2q9'
msg = 'hello,shatoshi'
secret = CBitcoinSecret(key)
address = P2PKHBitcoinAddress.from_pubkey(secret.pub)
message = BitcoinMessage(msg)

# 生成签名
signature = SignMessage(secret, message)

print('Address: %s' % address)
print('Message: %s' % msg)
print('Signature: %s' % signature)

# 验证签名
print('Verified: %s' % VerifyMessage(address, message, signature))