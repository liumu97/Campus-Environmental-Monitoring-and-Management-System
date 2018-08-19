from django.shortcuts import render, redirect
from django.http import HttpResponse,JsonResponse
from history import models
import templates
import time
import datetime
import json
import random


# Create your views here.
# 图表历史数据展示
def charts(request):
    # 差一个验证是不是GET方法
    context = {}  # 发回的数据
    data = []  # 一个传感器的数据
    today = datetime.date.today()
    start_date = today - datetime.timedelta(days=7)
    end_date = today - datetime.timedelta(seconds=30)
    sensor_type = ['pm', 'tem', 'hum', 'ray']
    # for item in sensor_type:  这个是错误代码，没搞懂原因
    #     # info = { }
    #     info['data'] = data_of_days(start_date, end_date, item)
    #     info['type'] = item
    #     print(info)
    #     data.append(info)
    #     print(data)
    # context['datas'] = data
    for item in sensor_type:
        info = {}  # 7天的数据
        info['data'] = data_of_days(start_date, end_date, item)
        print(data_of_days(start_date, end_date, item))
        info['type'] = item
        data.append(info)
    context['datas'] = data
    # 最近7条报警记录
    alarm_info = models.AlarminfoTb.objects.order_by('-alarm_time')[:7]
    alarm_detail = models.AlarmdetailTb.objects.all()
    alarm_data = []
    for item in alarm_info:
        message = {}
        message['time'] = item.alarm_time
        if item.alarmevent_type == 1:
            message['type'] = '火灾警报'
        else:
            message['type'] = '入侵警报'
        message['sensor'] = alarm_detail.get(alarmsensor_id=item.alarmsensor_id).alarm_sensor_name
        alarm_data.append(message)
    context['alarm'] = alarm_data
    # print(data)
    return JsonResponse(context)


# 安防记录查询
def security(request):
    if not request.session.get('is_login', None):
        return redirect('/login/')
    start_date = request.POST.get('start_date')
    end_date = request.POST.get('end_date')
    loginid = request.POST.get('login_id')
    context = {}
    data = []
    security_blog = models.SecurityBlogTb.objects.filter(access_date__gte=start_date, access_date__lte=end_date)
    if loginid:
        user_name = models.AuthorityrecordTb.objects.get(login_id=loginid).user_name
        rfid_id = models.AuthorityrecordTb.objects.get(login_id=loginid).schoolcard_num
        security_blog = security_blog.filter(access_cardid=rfid_id)
        for item in security_blog:
            info = { }    # 获取一天的数据
            info['username'] = user_name
            info['login_id'] = loginid
            info['arrive_time'] = str(item.access_date)
            data.append(info)
        context['datas'] = data
        # print(data)
        return render(request, 'guardlist.html', context)
    else:
        auth = models.AuthorityrecordTb.objects.all()
        for item in security_blog:
            info = { }
            auth_one = auth.get(schoolcard_num=item.access_cardid)
            info['arrive_time'] = str(item.access_date)
            info['login_id'] = auth_one.login_id
            info['username'] = auth_one.user_name
            data.append(info)
        context['datas'] = data
        return render(request, 'guardlist.html', context)


# 按系统类型查询历史记录
def system(request):
    if not request.session.get('is_login', None):
         return redirect('/login/')
    search_type = request.POST.get('search_type')
    start_date = request.POST.get('start_date')
    end_date = request.POST.get('end_date')
    context = {}   # 存放总的数据
    data = []      # 存放全部数据datas
    # info = {}      # 存放一条数据
    if search_type == 'alarminfo':
        alarm_info = models.AlarminfoTb.objects.filter(alarm_time__gte=start_date, alarm_time__lte=end_date)
        alarm_detail = models.AlarmdetailTb.objects.all()
        for item in alarm_info:
            info = { }     # 获取一天的数据
            info['date'] = str(item.alarm_time)
            if item.alarmevent_type == 1:
                info['alarm_type'] = '火灾'
            else:
                info['alarm_type'] = '入侵'
            info['sensor_id'] = item.alarmsensor_id
            info['sensor_name'] = alarm_detail.filter(alarmsensor_id=item.alarmsensor_id)[0].alarm_sensor_name
            # print(info['sensor_name'])
            data.append(info)
        context['datas'] = data
        return render(request, 'alarmlist.html', context)
    elif search_type =='security':
        security_blog = models.SecurityBlogTb.objects.filter(access_date__gte=start_date, access_date__lte=end_date)
        auth = models.AuthorityrecordTb.objects.all()
        for item in security_blog:
            info = { }
            info['date'] = str(item.access_date)
            info['access_card'] = item.access_cardid
            info['is_correct'] = item.is_correct
            info['username'] = auth.filter(schoolcard_num=item.access_cardid)[0].user_name
            data.append(info)
        context['datas'] = data
        return render(request, 'seculist.html', context)
    elif search_type == 'park':
        arrive = models.ParkingRecordTb.objects.filter(arrive_time__gte=start_date, arrive_time__lte=end_date)
        auth = models.AuthorityrecordTb.objects.all()
        for item in arrive:
            info = { }
            info['park_id'] = item.parkingcard_id
            info['arrive_time'] = str(item.arrive_time)
            info['leave_time'] = str(item.leave_time)
            info['username'] = auth.filter(schoolcard_num=item.parkingcard_id)[0].user_name
            data.append(info)
        context['datas'] = data
        return render(request, 'parklist.html', context)
    elif search_type == 'room':
        room = models.RoomEnvdataTb.objects.filter(env_date__gte=start_date, env_date__lte=end_date)
        for item in room:
            info = { }
            info['date'] = str(item.env_date)
            info['envtem'] = item.indoor_env_tem
            info['envhum'] = item.indoor_env_hum
            data.append(info)
        context['datas'] = data
        return render(request, 'roomlist.html', context)
    elif search_type == 'weather':
        weather = models.WeatherDataTb.objects.filter(wea_date1__gte=start_date, wea_date1__lte=end_date)
        for item in weather:
            info = { }
            info['date'] = str(item.wea_date1)
            info['light'] = item.light
            info['co2'] = item.co2
            info['tem'] = item.outdoor_tem_data
            info['hum'] = item.outdoor_hum_data
            info['ray'] = item.ultra_ray
            data.append(info)
        context['datas'] = data
        # print(data)
        return render(request, 'weatherlist.html', context)


# 按传感器类型查询历史记录
def sensor(request):
    if not request.session.get('is_login', None):
         return redirect('/login/')
    sensor_type = request.POST.get('sensor_type')
    statrt_date = request.POST.get('start_date')
    end_date = request.POST.get('end_date')
    context = {}    # 发的数据
    data = []       # 存放全部数据
    info = {}       # 一条数据
    context['sensor_type'] = sensor_type
    weatherdata = models.WeatherDataTb.objects.filter(wea_date1__lte=end_date, wea_date1__gte=statrt_date)
    weatherdata_1 = models.WeatherData2Tb.objects.filter(wea_date2__lte=end_date, wea_date2__gte=statrt_date)
    roomdata = models.RoomEnvdataTb.objects.filter(env_date__lte=end_date, env_date__gte=statrt_date)
    if sensor_type == 'light':
        for item in weatherdata:
            info = { }
            info['date'] = str(item.wea_date1)
            info['value'] = item.light
            info['sensor_type'] = '光照传感器'
            data.append(info)
    elif sensor_type == 'co2':
        for item in weatherdata:
            info = { }
            info['date'] = str(item.wea_date1)
            info['value'] = item.co2
            info['sensor_type'] = 'CO2传感器'
            data.append(info)
    elif sensor_type == 'tem':
        for item in weatherdata:
            info = { }
            info['date'] = str(item.wea_date1)
            info['value'] = item.outdoor_tem_data
            info['sensor_type'] = '温度传感器'
            data.append(info)
    elif sensor_type == 'hum':
        for item in weatherdata:
            info = { }
            info['date'] = str(item.wea_date1)
            info['value'] = item.outdoor_hum_data
            info['sensor_type'] = '湿度传感器'
            data.append(info)
    elif sensor_type == 'ray':
        for item in weatherdata:
            info = { }
            info['date'] = str(item.wea_date1)
            info['value'] = item.ultra_ray
            info['sensor_type'] = '紫外线传感器'
            data.append(info)
    elif sensor_type == 'pm':
        for item in weatherdata_1:
            info = { }
            info['date'] = str(item.wea_date2)
            info['value'] = item.pm_data
            info['sensor_type'] = 'PM2.5'
            data.append(info)
    elif sensor_type == 'envtem':
        for item in roomdata:
            info = { }
            info['date'] = str(item.env_date)
            info['value'] = item.indoor_env_tem
            info['sensor_type'] = '室内温度传感器'
            data.append(info)
    elif sensor_type == 'envhum':
        for item in roomdata:
            info = { }
            info['date'] = str(item.env_date)
            info['value'] = item.indoor_env_hum
            info['sensor_type'] = '室内湿度传感器'
            data.append(info)
    context['datas'] = data
    # print(data)
    return render(request, 'sensorlist.html', context)



# 将一天的数据浓缩成24个数据
def data_of_day(some_day, sensor_type): # d:表示时间的字符串，如Datetime类型
    context = {}  # 存储一天的24个数据
    data = []
    for i in range(0, 24):
        summary = 0  # 总和
        num = 0  # 数据个数
        if i <= 9:  # 将i转化为字符串，个位数时补0
            i = '0' + str(i)
        else:
            i = str(i)
        weatherdata = models.WeatherDataTb.objects.filter(wea_date1__regex=r'^'+str(some_day)+' '+i + '.+$')
        roomdata = models.RoomEnvdataTb.objects.filter(env_date__regex=r'^' + str(some_day) + ' ' + i + '.+$')
        weatherdata_1 = models.WeatherData2Tb.objects.filter(wea_date2__regex=r'^' + str(some_day) + ' ' + i + '.+$')
        if sensor_type == 'pm':
            for j in weatherdata_1:
                summary += j.pm_data
                num += 1
        elif sensor_type == 'envtem':
            for j in roomdata:
                summary += j.indoor_env_tem
                num += 1
        elif sensor_type == 'envhum':
            for j in roomdata:
                summary += j.indoor_env_hum
                num += 1
        elif sensor_type == 'light':
            for j in weatherdata:
                summary += j.light
                num += 1
        elif sensor_type == 'co2':
            for j in weatherdata:
                summary += j.co2
                num += 1
        elif sensor_type == 'tem':
            for j in weatherdata:
                summary += j.outdoor_tem_data
                num += 1
        elif sensor_type == 'hum':
            for j in weatherdata:
                summary += j.outdoor_hum_data
                num += 1
        elif sensor_type == 'ray':
            for j in weatherdata:
                summary += j.ultra_ray
                num += 1
        if num == 0:
            data.append(None)
        else:
            data.append(summary/num)
    # 一天的24个数据浓缩成4个
    i = 0
    num = 0
    ave = []  # 一天只留4个数据
    summary = 0
    for item in data:
        if item:
            summary += item
            num += 1
        if i == 3:
            i = 0
            if num:
                ave.append(summary/num)
                num = 0
                summary = 0
            else:
                ave.append(None)
        else:
            i += 1
            # ave.append(None)
    context['time'] = some_day
    context['data'] = ave
    return context


# 将几天的数据浓缩一哈
def data_of_days(start_day, end_day, sensor_type):
    data = []   # 直接发到前端的数据
    # #start = time.strptime(start_day, "%Y-%m-%d")  # 格式化时间，方便单独取年月日
    # #end = time.strptime(end_day, "%Y-%m-%d")
    # weatherdata = WeatherDataTb.objects.filter(wea_date__gte=start_day,wea_date__lte=end_day)
    start_day = datetime.datetime.strptime(str(start_day), "%Y-%m-%d")
    end_day = datetime.datetime.strptime(str(end_day), "%Y-%m-%d")
    while start_day.__le__(end_day):
        # data[str(start_day)] = data_of_day(start_day, sensor_type)
        data.append(data_of_day(str(start_day)[:10], sensor_type))
        start_day = start_day + datetime.timedelta(days=1)
    return data

# now = datetime.datetime.now()
# before = now - datetime.timedelta(days=7)
# i = 0
# while before.__lt__(now):
#     light = random.randint(500, 1000)
#     co2 = random.randint(1000, 3500)
#     out_tem = random.uniform(25, 32)
#     out_tem = round(out_tem, 1)
#     out_hum = random.randint(40, 80)
#     ray = random.uniform(1, 6)
#     ray = round(ray, 1)
#     pm = random.randint(1, 6)
#     in_tem = random.randint(20, 30)
#     in_hum = random.randint(30, 70)
#     models.WeatherDataTb.objects.create(wea_date1=before, light=light, co2=co2, outdoor_tem_data=out_tem, outdoor_hum_data=out_hum, ultra_ray=ray)
#     print(before)
#     print('-------Weather-1'+'----'+str(i))
#     models.WeatherData2Tb.objects.create(pm_data=pm, wea_date2=before)
#     models.RoomEnvdataTb.objects.create(env_date=before, indoor_env_tem=in_tem, indoor_env_hum=in_hum)
#     before = before + datetime.timedelta(minutes=3)
#     i +=1













