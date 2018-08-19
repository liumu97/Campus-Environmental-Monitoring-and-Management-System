from django.shortcuts import render, redirect
from django.http import HttpResponse,JsonResponse
# from history import models
from history.models import *

import socket
import threading
import json

# Create your views here.
# 定义socket
# Host = ''
# Port = 7777
# Addr = (Host, Port)
# sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# sock.connect(Addr)


# 控制开关类
def switch(request):
    if not request.session.get('is_login', None):
         return redirect('/login/')
    if request.method == 'POST':
        context = {}
        type = request.POST.get('type')
        command = request.POST.get('on_off')
        if type == 'fan':
            context['pack_type'] = "1"
            context['Fan_type'] = str(command)
            StatusfeedbackTb.objects.update(fan_status=command)
        elif type == 'curtain':
            context['pack_type'] = "2"
            context['Curtain_type'] = str(command)
            StatusfeedbackTb.objects.update(curtain_status=command)
        elif type == 'power':
            context['pack_type'] = "4"
            context['Power_type'] = str(command)
            StatusfeedbackTb.objects.update(pow_status=command)
        # 将数据发送给已经连接上来的网关
        for thread in threads:
            print('send msg!')
            print(context)
            try:
                thread.sock.send((json.dumps(context)+" END").encode())
            except Exception:
                thread.sock.close()
                threads.remove(thread)
            #thread.sock.send((json.dumps(send_data)).encode())
        print('\n------------------------------------------')
        print('已执行操作,switch')
        return JsonResponse({'res': 1})


# 控制彩灯
def light(request):
    if not request.session.get('is_login', None):
         return redirect('/login/')
    if request.method == 'POST':
        context = {}
        command = request.POST.get('type')
        context['pack_type'] = "6"
        context['Light_mode'] = str(command)
        print('---------------'+command+'-----------------------')
        StatusfeedbackTb.objects.update(fcled_status=command)
        for thread in threads:
            print('send msg!')
            thread.sock.send((json.dumps(context)+" END").encode())
            #thread.sock.send((json.dumps(send_data)).encode())
        print('\n------------------------------------------')
        print('已执行操作,彩灯')
        return JsonResponse({'res': 1})


# 控制门
def gate(request):
    if not request.session.get('is_login', None):
         return redirect('/login/')
    if request.method == 'POST':
        context = {}
        type = request.POST.get('type')
        if type == 'school':
            context['pack_type'] = "5"
            context['Schoolgate_type'] = '1'
        else:
            context['pack_type'] = "3"
            context['Parkgate_type'] = '1'
        # return HttpResponseRedirect("/login/send/?pack_type={}&".format(context['pack_type']))
        for thread in threads:
            print('send msg!')
            thread.sock.send((json.dumps(context)+" END").encode())
            #thread.sock.send((json.dumps(send_data)).encode())
        print('\n------------------------------------------')
        print('已执行操作,门')
        return JsonResponse({'res': 1})


# 控制照明灯
def lights(request):
    if not request.session.get('is_login', None):
         return redirect('/login/')
    if request.method == 'POST':
        context = {}
        room_num = request.POST.get('classroom')
        command = request.POST.get('on_off')
        bright = request.POST.get('bright')
        context['pack_type'] = "7"
        context['Room_num'] = str(room_num)
        context['Light_num'] = str(bright)
        if room_num == '1':
            StatusfeedbackTb.objects.update(ledlight1_status=command)
        elif room_num == '2':
            StatusfeedbackTb.objects.update(ledlight2_status=command)
        else:
            StatusfeedbackTb.objects.update(ledlight3_status=command)
        for thread in threads:
            print('send msg!')
            thread.sock.send((json.dumps(context)+" END").encode())
            #thread.sock.send((json.dumps(send_data)).encode())
        print('\n------------------------------------------')
        print('已执行操作，照明灯')
        return JsonResponse({'res': 1})


# 警报解除
def alarm(request):
    if not request.session.get('is_login', None):
         return redirect('/login/')
    if request.method == 'POST':
        context = {}
        type = request.POST.get('type')
        context['pack_type'] = "8"
        if type == '2':
            StatusfeedbackTb.objects.update(alarmbell_status=0, alarmlight_status=0)
        context['Alarm_type'] = str(type)
        for thread in threads:
            print('send msg!')
            thread.sock.send((json.dumps(context)+" END").encode())
            #thread.sock.send((json.dumps(send_data)).encode())
        print('\n------------------------------------------')
        print('已执行操作，警报解除')
    return JsonResponse({'res': 1})


# 微信控制按钮信息下发，除了警报按钮
def wx_control(request):
    # if not request.session.get('is_login', None):
    #      return redirect('/login/')
    if request.method == 'POST':
        context = {}
        message = ''   # 控制台打印提示信息
        pack_type = request.POST.get('pack_type')
        message = pack_type
        if pack_type == '1':
            print('发送switch信息')
            button_type = request.POST.get('button_type')
            on_off = request.POST.get('on_off')
            print(on_off)
            if on_off == 'true':
                command = 1
            else:
                command = 0
            if button_type == 'fan':
                context['pack_type'] = "1"
                context['Fan_type'] = str(command)
                StatusfeedbackTb.objects.update(fan_status=command)
            elif button_type == 'curtain':
                context['pack_type'] = "2"
                context['Curtain_type'] = str(command)
                StatusfeedbackTb.objects.update(curtain_status=command)
            elif button_type == 'power':
                context['pack_type'] = "4"
                if command == 0:
                    command = 1
                else:
                    command = 2
                context['Power_type'] = str(command)
                StatusfeedbackTb.objects.update(pow_status=command)
        elif pack_type == '2':
            print('发送light信息')
            context['pack_type'] = "6"
            context['Light_mode'] = str(request.POST.get('button_type'))
            StatusfeedbackTb.objects.update(fcled_status=request.POST.get('button_type'))
        elif pack_type == '3':
            print('发送gate信息')
            button_type = request.POST.get('button_type')
            if button_type == 'school':
                print("school")
                context['pack_type'] = "5"
                context['Schoolgate_type'] = '1'
            else:
                print("park")
                context['pack_type'] = "3"
                context['Parkgate_type'] = '1'
        elif pack_type == '4':
            print('发送lights信息')
            context['pack_type'] = "7"
            button_type = request.POST.get('button_type')
            context['Room_num'] = str(button_type)
            bright = int(request.POST.get('bright'))
            bright = bright - bright % 10
            print(bright)
            context['Light_num'] = str(bright)
            if bright == '0':
                command = 0
            else:
                command = 1
            if button_type == '1':
                print("77777")
                StatusfeedbackTb.objects.update(ledlight1_status=command)
            elif button_type == '2':
                StatusfeedbackTb.objects.update(ledlight2_status=command)
            else:
                StatusfeedbackTb.objects.update(ledlight3_status=command)
        for thread in threads:
            print('send msg!')
            thread.sock.send((json.dumps(context) + " END").encode())
        print('\n------------------------------------------')
        print('已微信执行操作:'+pack_type)
        return JsonResponse({'res': 1})


# 微信解除警报
def wx_alarm(request):
    if request.method == 'POST':
        context = {}
        button_type = request.POST.get('button_type')
        # command = request.POST.get('on_off')   # 暂时还只能关
        context['pack_type'] = "8"
        context['Alarm_type'] = str(button_type)
        if button_type == '2':
            print('入侵警报')
            StatusfeedbackTb.objects.update(alarmbell_status=0, alarmlight_status=0)
        for thread in threads:
            print('send msg!')
            thread.sock.send((json.dumps(context)+" END").encode())
            #thread.sock.send((json.dumps(send_data)).encode())
        print('\n------------------------------------------')
        print('已执行微信操作，警报解除')
        return JsonResponse({'res': 1})


# 控制指纹
def fingerprint(request):
    if request.method == 'POST':
        context = {}
        context['pack_type'] = '10'
        context['Finger_type'] = request.POST.get('type')
        for thread in threads:
            print('send msg!')
            thread.sock.send((json.dumps(context) + " END").encode())
        print('\n------------------------------------------')
        print('已执行操作，指纹')
        return JsonResponse({'res': 1})










# 接收网关传来的数据
HOST = '0.0.0.0'
PORT = 7777
ADDR = (HOST, PORT)

buffer_size = 1024
# tz = pytz.timezone('Asia/Shanghai')
# # -------------------------socket-----------------------
# def  change_tz(time):
#     return datetime.datetime.strptime(time, '%Y-%m-%d %H:%M:%S').replace(tzinfo=tz)


def recv_sockdata(the_socket):
    recv_data = ""
    while True:
        try:
            data = the_socket.recv(buffer_size).decode()
        except ConnectionAbortedError or OSError:
            pass
        if 'END' in data:
            recv_data += data[:data.index('END')]
            break
        recv_data += data
    return recv_data


def save_data(json_data, data_type, the_sock):
    if data_type == '1':
        # json_data['wea_date'] = change_tz(json_data['wea_date'])
        new_weather = WeatherDataTb(
            wea_date1        = json_data['wea_date'],
            light            = json_data['light'],
            co2              = json_data['co2'],
            outdoor_tem_data = json_data['tem_data'],
            outdoor_hum_data = json_data['hum_data'],
            ultra_ray        = json_data['ultra_ray']
        )
        new_weather.save()
        cur_condition = StatusfeedbackTb.objects.all()[0].curtain_status
        if int(json_data['ultra_ray']) >= 5 and int(cur_condition) == 0:
            context = {}
            context['pack_type'] = '2'
            context['Curtain_type'] = '1'
            the_sock.send((json.dumps(context)+" END").encode())
            StatusfeedbackTb.objects.update(curtain_status=1)
        elif int(json_data['ultra_ray']) <= 2 and int(cur_condition) == 1:
            context = {}
            context['pack_type'] = '2'
            context['Curtain_type'] = '0'
            the_sock.send((json.dumps(context) + " END").encode())
            StatusfeedbackTb.objects.update(curtain_status=0)

    elif data_type == '2':
        # json_data['wea_date'] = change_tz(json_data['wea_date'])
        new_weather2 = WeatherData2Tb(
            wea_date2= json_data['wea_date'],
            pm_data  = json_data['pm_data']
        )
        new_weather2.save()

    elif data_type == '3':
        # json_data['access_date'] = change_tz(json_data['access_date'])
        staff =  AuthorityrecordTb.objects.filter(schoolcard_num=json_data['access_cardid'])
        if not staff:
            json_data['is_correct'] = 0
            print('非法用户')
            return 4
        else:
            json_data['is_correct'] = 1
            print('合法用户')
            new_secu = SecurityBlogTb(
                access_date  = json_data['access_date'],
                access_cardid= json_data['access_cardid'],
                is_correct   = json_data['is_correct']
            )
            new_secu.save()
            return 5

    elif data_type == '4':
        # json_data['env_date'] = change_tz(json_data['env_date'])
        new_indoorenv = RoomEnvdataTb(
            env_date      = json_data['env_date'],
            indoor_env_hum= json_data['env_hum'],
            indoor_env_tem= json_data['env_tem']
        )
        new_indoorenv.save()

    elif data_type == '5':
        #搜索是否有停车
        time = json_data['time']
        # json_data['time'] = change_tz(time)
        is_our = AuthorityrecordTb.objects.filter(schoolcard_num=json_data['parkingcard_id'])
        if not is_our:
            return 2
        park_tuple = ParkingRecordTb.objects.filter(parkingcard_id=json_data['parkingcard_id'], operation=0)
        #判断是否为空
        if not park_tuple:
            print('没有该记录，重新存储')
            new_parking = ParkingRecordTb(
                parkingcard_id= json_data['parkingcard_id'],
                arrive_time=    json_data['time'],
                operation= 0
            )
            new_parking.save()

        else:
            print('已有该记录，更新出库')
            ParkingRecordTb.objects.filter(parkingcard_id=json_data['parkingcard_id'],
                                           operation=0).update(leave_time=json_data['time'], operation=1)
        return 3


    elif data_type == '6':
        # json_data['alarm_time'] = change_tz(json_data['alarm_time'])
        new_alaminfo = AlarminfoTb(
            alarm_time         = json_data['alarm_time'],
            alarmevent_type    = json_data['alarmevent_type'],
            alarmsensor_id     = json_data['alarmsensor_id'],
            event_handle_status= 0

        )
        new_alaminfo.save()
        StatusfeedbackTb.objects.all().update(alarmlight_status=1, alarmbell_status=1 )

    else:
        print('error data', json_data)
        return 0
    return 1



class MyThread(threading.Thread, object):

    def __init__(self, sock, addr):
        threading.Thread.__init__(self)
        self.sock = sock
        self.addr = addr

    def run(self):
        print('xxxxxxx'.center(50, '-'))
        print('Accept new connection from {}...'.format(self.addr))
        self.recv_data()
        print('Connection from {} closed.'.format(self.addr))

    def recv_data(self):
        while True:
            try:
                res_data = recv_sockdata(self.sock)
            except ConnectionAbortedError and ConnectionResetError:
                self.sock.close()
                threads.remove(self.sock)
            try:
                json_data = json.loads(res_data, encoding="utf-8")
                data_type = json_data['type']
                print(type(json_data), json_data)  # 收到的数据
                ack = save_data(json_data, data_type, self.sock)
                if ack == 1:
                    pass
                elif ack == 2:
                    send_back = {}
                    send_back['pack_type'] = 9
                    send_back['Is_success'] = 2
                    send_back['Card_type'] = 1
                    data = json.dumps(send_back)
                    data = data + 'END'
                    self.sock.send(bytes(data, encoding='utf-8'))
                elif ack == 3:
                    send_back = {}
                    send_back['pack_type'] = 9
                    send_back['Is_success'] = 1
                    send_back['Card_type'] = 1
                    data = json.dumps(send_back)
                    data = data + 'END'
                    self.sock.send(bytes(data, encoding='utf-8'))
                elif ack == 4:
                    send_back = {}
                    send_back['pack_type'] = 9
                    send_back['Is_success'] = 2
                    send_back['Card_type'] = 2
                    data = json.dumps(send_back)
                    data = data + 'END'
                    self.sock.send(bytes(data, encoding='utf-8'))
                elif ack == 5:
                    send_back = {}
                    send_back['pack_type'] = 9
                    send_back['Is_success'] = 1
                    send_back['Card_type'] = 2
                    data = json.dumps(send_back)
                    data = data + 'END'
                    self.sock.send(bytes(data, encoding='utf-8'))
                else:
                    self.sock.send(b'Error package type')

            except json.decoder.JSONDecodeError as e:
                print("error data:" + res_data)
                self.sock.send(b'Error json code')
                continue


# 创建Socket
tcpSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpSocket.bind(ADDR)
tcpSocket.listen(16)
threads = []
print('Waiting for connection...'.center(50, '-'))


def do_service():
    while True:
        # 接受一个新连接:
        sock, addr = tcpSocket.accept()
        #---------------------------test for lll--------------------------

        # json_data = {}
        # json_data['pack_type'] = '1'
        # json_data['Fan_type'] = '1'
        #
        # data = json.dumps(json_data)
        # data = data + 'END'
        # sock.send(bytes(data, encoding='utf-8'))
        # print('successfully send back')


        #----------------------------------------------------------------
        mthread = MyThread(sock, addr)
        mthread.start()
        threads.append(mthread)

# 将阻塞接受新连接的逻辑放在线程中
threading.Thread(target=do_service).start()


# #接收网关数据。存储到服务器数据库
#
# def recv_sockdata(the_socket):
#
#     # 因为socket每次收到的不一定是完整的数据帧，所以我这里规定的协议，收到END作为一段完整数据的结尾。因此这个函数循环读入数据，返回读到的完整数据帧。
#
#     total_data = ""
#
#     while True:
#
#         data = the_socket.recv(1024).decode()
#
#         if "END" in data:
#
#             total_data += data[:data.index("END")]
#
#             break
#
#         total_data += data
#
#     # print(total_data)
#
#     # print("-----------------")
#
#     return total_data
#
# server_addr = ('0.0.0.0', 7777)
#
# # 线程类，处理和网关的数据交互
#
# class MyThread(threading.Thread,object):
#
#     def __init__(self, sock, addr):
#
#         threading.Thread.__init__(self)
#
#         self.sock = sock
#
#         self.addr = addr
#
#     def run(self):
#
#         print('Accept new connection from {}...'.format(self.addr))
#
#         self.link_to_send()
#
#         self.recv_data()
#
#         print('Connection from {} closed.'.format(self.addr))
#
#     def recv_data(self):
#
#         while True:
#
#             res_data = recv_sockdata(self.sock)
#
#             try:
#
#                 data = json.loads(res_data, encoding="utf-8")
#
#                 # 这个地方的data就是网关传上来的数据，可以在此完成数据入库的操作，或者在此将数据推送到前端。
#
#                 print(data)# 收到的数据
#
#             except json.decoder.JSONDecodeError as e:
#
#                 print("error data:"+res_data)
#
#                 continue
#
#     def link_to_send(self):  # 当有连接时，返回一个hello
#         send_data = {}
#         send_data['time'] = time.asctime(time.localtime(time.time()))
#         send_data['direction'] = 'DOWN'
#         send_data['message'] = 'Welcome to visit Server !'
#         send_data['PS'] = 'YHS  NB !!!!!!'
#         self.sock.send((json.dumps(send_data)).encode())
#         print('OK~~----------')
#
# #创建Socket
#
# tcpSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#
# #绑定地址
#
# tcpSocket.bind(server_addr)
#
# #监听端口，传入的参数指定等待连接的最大数量
#
# tcpSocket.listen(16)
#
# threads = []
#
# print('Waiting for connection...')
#
# #服务器程序通过一个永久循环来接受来自客户端的连接
#
# def do_service():
#
#     while True:
#
#         # 接受一个新连接:
#
#         sock, addr = tcpSocket.accept()
#
#         # 创建新线程来处理TCP连接:每个连接都必须创建新线程（或进程）来处理，
#
#         # 否则，单线程在处理连接的过程中，无法接受其他客户端的连接：
#
#         mthread = MyThread(sock, addr)
#
#         mthread.start()
#
#         threads.append(mthread)
#
# # 将阻塞接受新连接的逻辑放在线程中
#
# threading.Thread(target=do_service).start()
#
# # 这个control就是普通的视图函数，和大家平时写的index，login函数一样。
#
# def control(request):
#
#     print(request.POST)
#
#     # 这里从POST收到了你前端的数据。
#
#     # 把数据进行整理后（这里模拟了数据），通过与网关的连接发送给网关。
#
#     send_data = {}
#
#     send_data['gateid'] = request.POST.get('gateid')
#
#     send_data['direction'] = 'DOWN'
#
#     send_data['nodeid'] = request.POST.get('nodeid')
#
#     send_data['type'] = request.POST.get('type')
#
#     send_data['data'] = request.POST.get('data')
#
#     send_data['time'] = request.POST.get('time')
#
#     # 将数据发送给已经连接上来的网关
#
#     for thread in threads:
#
#         print('send msg!')
#
#         # thread.sock.send((json.dumps(send_data)+" END").encode())
#         thread.sock.send((json.dumps(send_data)).encode())
#     return HttpResponse(status=200, content='OK')
