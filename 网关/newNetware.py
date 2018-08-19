# -*- coding:utf-8 -*-
__author__ = 'Threedog'
__Date__ = '2018/7/19 11:19'

'''
列举可用的端口名
import serial.tools.list_ports
plist = list(serial.tools.list_ports.comports())
if len(plist) <= 0:
    print("没有发现端口!")
else:
    plist_0 = list(plist[0])
    serialName = plist_0[0]
    serialFd = serial.Serial(serialName, 9600, timeout=60)
    print("可用端口名>>>", serialFd.name)
'''

# pip install pyserial 安装串口相关包
import serial # 串口
import socket # 网络
import json
import random
import time
import threading
flag = 1
mflag = [0,]
# 串口
# ubuntu下串口为/dev/ttyS0 /dev/ttyS1等
# read(size=1)            # 从串口读取size个字节
# write(s)                # 把字符串s写到该端口
# flushInput()            # 清除输入缓存区，放弃所有内容
# flushOutput()           # 清除输出缓冲区，放弃输出
mserial = serial.Serial(port='/dev/ttyAMA0',baudrate=57600,bytesize=8,parity='N',stopbits=1,timeout=2) # 打开串口，配置串口
# 网络
tcp_client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server_addr = ('134.175.40.47',7777) # 服务器ip和端口号。
tcp_client.connect(server_addr)



# 处理网络收到数据
def recv_sockdata(the_socket):
    total_data = ""
    while True:
        data = the_socket.recv(1024).decode()
        print(data)
        if "END" in data:
            total_data += data[:data.index("END")]
            break
        total_data += data
    # print(total_data)
    # print("-----------------")
    return total_data


# 字符串转16进制，用于处理串口接收到的数据
def convert_hex(string):
    res = []
    result = []
    for item in string:
        res.append(item)
    for i in res:
        result.append(hex(i))
    return result


# 从网络获取数据， 死循环阻塞执行
def recv_sock_data():
    global flag
    while True:
        try:
            if not flag:
                break
            resv_data = recv_sockdata(tcp_client)
            print("receive data : {}".format(resv_data))
            # 接收到网络数据后，协议转换，然后通过串口发送给硬件，这里模拟一条数据发送：
            # 发送十六进制字符串010591F50000F104
            # 对应的
            user_dict = json.loads(resv_data)
            cmd1 =  bytearray(b'\xbb\xaa\x00\xFF\xFF')
            cmd3 =  bytearray(b'\xEE\xFF')
            if user_dict['pack_type'] == '1':#风扇控制包
                if user_dict['Fan_type'] == '1':#开风扇
                    cmd2 = bytearray(b'\x02\x07\x01\x01\x01')
                elif user_dict['Fan_type'] == '0':#关风扇
                    cmd2 = bytearray(b'\x02\x07\x01\x01\x00')
            elif user_dict['pack_type'] == '2':#自动窗帘控制包
                if user_dict['Curtain_type'] == '1':#开窗帘
                    cmd2 = bytearray(b'\x02\x07\x04\x01\x00')
                elif user_dict['Curtain_type'] == '0':#关窗帘
                    cmd2 = bytearray(b'\x02\x07\x04\x01\x01')
            elif user_dict['pack_type'] == '3':#停车场大门控制包
                cmd2 = bytearray(b'\x02\x06\x01\x01\x01')
            elif user_dict['pack_type'] == '4':#电源切换包
                if user_dict['Power_type'] == '1':#切换到正常电源
                    cmd2 = bytearray(b'\x02\x07\x02\x01\x00')
                elif user_dict['Power_type'] == '2':#切换到备用电源
                    cmd2 = bytearray(b'\x02\x07\x02\x01\x01')
            elif user_dict['pack_type'] == '5':#学校大门控制包（开门）
                cmd2 = bytearray(b'\x02\x08\x01\x01\x00')
            elif user_dict['pack_type'] == '6':#室外景观（炫彩）灯控制包
                if user_dict['Light_mode'] == '1':#模式一
                    cmd2 = bytearray(b'\x02\x01\x02\x01\x01')
                elif user_dict['Light_mode'] == '2':#模式二
                    cmd2 = bytearray(b'\x02\x01\x02\x01\x02')
                elif user_dict['Light_mode'] == '3':#模式三
                    cmd2 = bytearray(b'\x02\x01\x02\x01\x03')
                elif user_dict['Light_mode'] == '0':#关闭
                    cmd2 = bytearray(b'\x02\x01\x02\x01\x04')
            elif user_dict['pack_type'] == '7':#室内照明灯控制包
                if user_dict['Room_num'] == '1':#教室编号
                    cmd2 = bytearray(b'\x02\x01\x01\x02\x02')
                elif user_dict['Room_num'] == '2':
                    cmd2 = bytearray(b'\x02\x01\x01\x02\x04')
                elif user_dict['Room_num'] == '3':
                    cmd2 = bytearray(b'\x02\x01\x01\x02\x08')
                #print(user_dict['Light_num'])
                if user_dict['Light_num'] == '0':
                    cmd2 += bytearray(b'\x00')
                elif user_dict['Light_num'] == '10':
                    cmd2 += bytearray(b'\x0A')
                elif user_dict['Light_num'] == '20':
                    cmd2 += bytearray(b'\x14')
                elif user_dict['Light_num'] == '30':
                    cmd2 += bytearray(b'\x1E')
                elif user_dict['Light_num'] == '40':
                    cmd2 += bytearray(b'\x28')
                elif user_dict['Light_num'] == '50':
                    cmd2 += bytearray(b'\x32')
                elif user_dict['Light_num'] == '60':
                    cmd2 += bytearray(b'\x3C')
                elif user_dict['Light_num'] == '70':
                    cmd2 += bytearray(b'\x46')
                elif user_dict['Light_num'] == '80':
                    cmd2 += bytearray(b'\x50')
                elif user_dict['Light_num'] == '90':
                    cmd2 += bytearray(b'\x5A')
                elif user_dict['Light_num'] == '100':
                    cmd2 += bytearray(b'\x64')
                #cmd2 += bytearray(hex(int(user_dict['Light_num'])).encode())#亮度
            elif user_dict['pack_type'] == '8':#警报解除包
                if user_dict['Alarm_type'] == '1':#火灾警报
                    cmd2 = bytearray(b'\x02\x03\x00\x01\x00')
                elif user_dict['Alarm_type'] == '2':#入侵警报
                    cmd2 = bytearray(b'\x02\x05\x00\x01\x00')
            elif user_dict['pack_type'] == 9:#RFID匹配反馈包
                if user_dict['Is_success'] == 1:#刷卡成功
                    if user_dict['Card_type'] == 1:#停车场门禁
                        cmd2 = bytearray(b'\x02\x06\x03\x01\x01')
                    elif(user_dict['Card_type']) == 2:#学校大门门禁
                        cmd2 = bytearray(b'\x02\x08\x04\x01\x01')
                elif user_dict['Is_success'] == 2:#刷卡失败
                    if user_dict['Card_type'] == 1:#停车场门禁
                        cmd2 = bytearray(b'\x02\x06\x03\x01\x02')
                    elif user_dict['Card_type'] == 2:#学校大门门禁
                        cmd2 = bytearray(b'\x02\x08\x04\x01\x02')
            elif user_dict['pack_type'] == '10':#指纹识别控制包
                if user_dict['Finger_type'] == '0':#指纹录入
                    cmd2 = bytearray(b'\x02\x08\x02\x01\x00')
                elif user_dict['Finger_type'] == '1':#指纹识别
                    cmd2 = bytearray(b'\x02\x08\x02\x01\x01')
            cmd4 = 0
            for i in cmd2:
                cmd4 = i ^ cmd4
            cmd4 = bytearray([cmd4])
            cmd = cmd1+cmd2+cmd4+cmd3#拼装成标准的zigbee协议包

            print(cmd)
            mserial.write(cmd)#向串口发送包
            print("write")
            #print("write")
        except (Exception): #
            continue
    flag = 0


# 从串口获取数据，和网络一样放在死循环阻塞执行，两者必须有一个放在线程里执行，否则会造成阻塞

def recv_serial_data():

    global flag
    global mflag

    while True:
        try:
            if not flag:
                break
            # 接收数据：
            #tcp_client.sendall(("She is here! "+"END").encode())
            print("recv")

            resp=bytearray(b'')
            flag1 = True
            while flag1:
                current = mserial.read()
                resp += current
                if current == b'\xee':
                    current = mserial.read()
                    if current == b'\xff':
                        resp += current
                        flag1 = False
                    if  flag1 == True:
                        resp += current
            '''
            while True:
                current=mserial.read()
                print(current)
                resp += current
                if current == b'\xEE':
                    current =mserial.read()
                    if current == b'\xff':
                        resp += bytearray(b'\xff')
                        break
                    resp += current
                    '''
            #resp = mserial.read(14)

            #print(resp)
            resp1 = convert_hex(resp)
            print(resp1)
            mdict = {}
            if len(resp) == 0:#排除空的情况
                continue
            if resp[0] != 0xaa or resp[1] != 0xbb:#检验报头
                continue
            cmd5 = 0
            cmd6 = resp[5:9+resp[8]]
            print("jianyan1")

            for i in cmd6:#
                cmd5 = i ^ cmd5
            if cmd5 != resp[9+resp[8]]:#验证校验和
                continue
            print("jianyan2")
            if resp[10+resp[8]] != 0xee or resp[11+resp[8]] !=0xff:#判断帧尾和结束符
                continue
            print("jianyan3")
            if resp[5] == 0x03:#心跳帧
                mflag[int(resp[6])-1] -= 1
                continue
            elif resp[6] == 0x02:#天气监测数据包1
                mdict['type'] = '1'
                mdict['wea_date'] = time.strftime('%Y-%m-%d %H:%M:%S')
                mint1 = int(resp1[15], 16) * 256 + int(resp1[16], 16)
                mint1 /= 10.0
                mdict['light'] = mint1
                mint1 = int(resp1[9],16)*256 + int(resp1[10],16)
                mdict['co2'] = mint1
                mint1 = int(resp1[11], 16) * 256 + int(resp1[12], 16)
                mint1 /= 1000.0
                mdict['tem_data'] = mint1
                mint1 = int(resp1[13], 16) * 256 + int(resp1[14], 16)
                mint1 /= 100.0
                mdict['hum_data'] = mint1
                mint1 = int(resp1[17], 16) * 256 + int(resp1[18], 16)
                mdict['ultra_ray'] = mint1

            elif resp[6] == 0x04 and resp[7] == 0x01:#天气监测数据包2
                mdict['type'] = '2'
                mdict['wea_date'] = time.strftime('%Y-%m-%d %H:%M:%S')
                mint1 = int(resp1[9], 16) * 256 + int(resp1[10], 16)
                mint1 /= 100.0
                mdict['pm_data'] = mint1

            elif resp[6] == 0x08 and resp[7] == 0x04:#门禁人员访问记录包
                mdict['type'] = '3'
                mdict['access_date'] = time.strftime('%Y-%m-%d %H:%M:%S')
                if resp[9] == 0x9A:
                    mdict['access_cardid'] = '9A8E5C2F'
                else:
                    mdict['access_cardid'] = '86B1B412'
            elif resp[6] == 0x04 and resp[7] == 0x02:#房间环境数据包
                mdict['type'] = '4'
                mdict['env_date'] = time.strftime('%Y-%m-%d %H:%M:%S')
                mint1 = int(resp1[9], 16) * 256 + int(resp1[10], 16)
                mint1 /= 1000.0
                mdict['env_tem'] = mint1
                mint1 = int(resp1[11], 16) * 256 + int(resp1[12], 16)
                mint1 /= 100.0
                mdict['env_hum'] = mint1

            elif resp[6] == 0x06 and resp[7] == 0x03:#停车场门禁数据记录包
                mdict['type'] = '5'
                mdict['time'] = time.strftime('%Y-%m-%d %H:%M:%S')
                if resp[9] == 0x9A:
                    mdict['parkingcard_id'] = '9A8E5C2F'
                else:
                    mdict['parkingcard_id'] = '86B1B412'

            elif resp[6] == 0x03 :#火灾警报包
                mdict['type'] = '6'
                mdict['alarm_time'] = time.strftime('%Y-%m-%d %H:%M:%S')
                mdict['alarmevent_type'] = '1'
                if resp[7] == 0x01:
                    mdict['alarmsensor_id'] = '3'#烟雾传感器
                elif resp[7] == 0x02:
                    mdict['alarmsensor_id'] = '1'#甲烷传感器
                elif resp[7] == 0x03:
                    mdict['alarmsensor_id'] = '4'#红外传感器
                elif resp[7] == 0x04:
                    mdict['alarmsensor_id'] = '2'#火光传感器

            elif resp[6] == 0x05:#入侵警报包
                mdict['type'] = '6'
                mdict['alarm_time'] = time.strftime('%Y-%m-%d %H:%M:%S')
                mdict['alarmevent_type'] = '2'
                if resp[7] == 0x03:
                    mdict['alarmsensor_id'] = '5'#反射传感器
                elif resp[7] == 0x04:
                    mdict['alarmsensor_id'] = '6'#对射传感器

            elif resp[6] == 0x07:#电源切换警报包
                mdict['type'] = '6'
                mdict['alarm_time'] = time.strftime('%Y-%m-%d %H:%M:%S')
                mdict['alarmevent_type'] = '3'
                mdict['alarmsensor_id'] = '7'
            elif resp[6] == 0x08:
                mdict['type'] = '7'
                if resp[7] == 0x02:#指纹控制反馈包
                    if resp[9] == 0x00:#指纹录入成功
                        mdict['match_type'] = '0'
                    elif resp[9] == 0x01:#指纹识别成功
                        mdict['match_type'] = '1'
                    elif resp[9] == 0x02:#指纹识别失败
                        mdict['match_type'] = '2'
        except (Exception):
            continue
        # 接收到串口数据后，协议转换，然后通过网络发送给服务器，这里模拟一条数据发送：
        print((json.dumps(mdict) + " END").encode())
        tcp_client.sendall((json.dumps(mdict) + " END").encode())

        #cmd = bytearray(b'\xbb\xaa\x00\xFF\xFF\x02\x01\x01\x02\x02\x05\x07\xEE\xFF')
        #print("cmd")
        #mserial.write(cmd)
        #print("write")

    #flag = 0

def heart():
    global mflag
    while True:
        time.sleep(5)
        j = 0
        for i in mflag:
            print(i)
            if i > 2:
                print("cmd11")
                cmd11 = bytearray(b'\xbb\xaa\x00\xff\xff\x02\x07\x03\x01\x01\x06\xee\xff')
                mserial.write(cmd11)
                time.sleep(0.5)
                print("write")
                mflag[j] = 0
            j += 1
        cmd10 = bytearray(b'\xbb\xaa\x00\xff\xff\x03\x00\x00\x01\x00\x02\xee\xff')
        j = 0
        for i in mflag:
            i = i + 1
            mflag[j] = i
            j += 1
        mserial.write(cmd10)



#线程中启动心跳帧

#threading.Thread(target=heart).start()

# 线程中启动串口监听
threading.Thread(target=recv_serial_data).start()
# 主线程启动网络监听
#recv_serial_data()
recv_sock_data()
'''
print('ok')

tcp_svr = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcp_svr.bind(('0.0.0.0', 7777))
tcp_svr.listen(16)
print('ok')
tcp_client, addr = tcp_svr.accept()
print(addr)
print(recv_sock_data(tcp_client))
'''