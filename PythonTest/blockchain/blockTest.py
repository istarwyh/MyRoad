from Blockchain import Blockchain  # 从文件引入Blockchain类

blockchain = Blockchain()  # 创建新的 Blockchain 对象
print(blockchain.new_block("block1"))
print(blockchain.chain)
print(blockchain.last_block)
# 区块首尾相连
print(blockchain.new_block("block2"), blockchain.hash(blockchain.last_block))
print(blockchain.chain)

###########################################
blockchain = Blockchain()  # 创建新的 Blockchain 对象
print(blockchain.new_block("first bloooooooock"))  # 创世区块

# 插入两笔交易到交易池
blockchain.new_transaction("alice", "bob", 1000)
blockchain.new_transaction("bob", "zhangsan", 400)

print(blockchain.new_block("second", blockchain.hash(blockchain.last_block)))
print(blockchain.chain)
##########################################
b = Blockchain()
print(b.last_nonce)  # 查看当前 nonce
b.new_block(333)  # 现在随便输入一个 nonce 不行了
nonce = b.proof_of_work(b.last_nonce)  # 需要计算出合法的nonce才能生成新区块！
b.new_block(nonce)
