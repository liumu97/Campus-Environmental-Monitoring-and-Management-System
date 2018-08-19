from django.shortcuts import render    #返回渲染的页面
from django.http import HttpResponse, HttpResponseRedirect,JsonResponse
from django.shortcuts import redirect  #重定向页面
from history import models          #数据库用户表
from django.views.decorators.csrf import csrf_exempt
# import templates
# import socket
# import threading
import json



# Create your views here.

def index(request):
    if not request.session.get('is_login', None):
        return redirect("/")
    else:
        return render(request, "test_index.html")

@csrf_exempt
def login(request):
    if request.session.get('is_login', None):
        return redirect("/login/index/")
    if request.method == "POST":
        username = request.POST.get('username', None)
        password = request.POST.get('password', None)
        #username = username.strip() #消除字符串前后空的内容
        # #其他的对于帐号密码的验证
        # 用户名字符合法性验证
        # 密码长度验证
        try:
            user = models.AuthorityrecordTb.objects.get(login_id=username)
            if user.logpassword == password:
                #进入首页时，加上用户信息和session
                res = '11'    # 登录成功
                request.session['is_login'] = True
                request.session['auth'] = user.authority
                request.session['user_name'] = user.user_name
                return JsonResponse({'res': res})
            else:
                res = '01'   # 密码不正确
        except:
            res = '10'   # 用户名不存在！
        return JsonResponse({'res': res})
    return render(request, 'homepage.html')


def wx_login(request):
    if request.method == "POST":
        username = request.POST.get('username', None)
        password = request.POST.get('password', None)
        #username = username.strip() #消除字符串前后空的内容
        # #其他的对于帐号密码的验证
        # 用户名字符合法性验证
        # 密码长度验证
        try:
            user = models.AuthorityrecordTb.objects.get(login_id=username)
            print("777777")
            if user.logpassword == password:
                #进入首页时，加上用户信息和session
                print("666666")
                res = '11'    # 登录成功
                return HttpResponse(content_type=json, content=json.dumps({'res': res}))
            else:
                res = '01'   # 密码不正确
        except:
            res = '10'   # 用户名不存在！
    return HttpResponse(content_type=json, content=json.dumps({'res': res}))


def logout(request):
    if not request.session.get('is_login', None):
        return redirect('/login/')
    request.session.flush()
    #或者使用下面的方法
    #del request.session['is_login']
    return redirect('/login/')

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
#     def __init__(self,sock,addr):
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
#                 data = json.loads(res_data,encoding="utf-8")
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
#def control(request):
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
#
#
#

