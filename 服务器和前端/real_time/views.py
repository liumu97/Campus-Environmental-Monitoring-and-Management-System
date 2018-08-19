from django.shortcuts import render,HttpResponse, redirect
from django.http import JsonResponse
from history import models
import json

# Create your views here.
# 数据格式：{'temperature':'','hum':'',}


# 传感器实时数据
def present(request):
    # if not request.session.get('is_login', None):
    #     return redirect('/login/')
    if request.method == 'GET':
        context = {}   # 存放返回给浏览器的数据
        weather_data = models.WeatherDataTb.objects.order_by('-wea_date1')  # 按照时间降序排序
        weather_data_1 = models.WeatherData2Tb.objects.order_by('-wea_date2')
        room_data = models.RoomEnvdataTb.objects.order_by('-env_date')
        latest = weather_data[0]   # 第一个对象就是最新的数据
        latest_1 = weather_data_1[0]
        latest_2 = room_data[0]
        context['tem'] = latest.outdoor_tem_data  #取对应的数据，存入context字典
        context['light'] = latest.light
        context['co2'] = latest.co2
        context['hum'] = latest.outdoor_hum_data
        context['ray'] = latest.ultra_ray
        context['pm'] = latest_1.pm_data
        context['envtem'] = latest_2.indoor_env_tem
        context['envhum'] = latest_2.indoor_env_hum
        print(context)
        # 以json格式返回给浏览器
        return HttpResponse(content_type=json, content=json.dumps(context))


# 执行器实时数据
def execute(request):
    # if not request.session.get('is_login', None):
    #     return redirect('/login/')
    if request.method == 'GET':
        context = {}
        # status_feedback = models.StatusfeedbackTb.objects.order_by('-wea_date')
        status_feedback = models.StatusfeedbackTb.objects.all()
        latest = status_feedback[0]
        context['fan'] = latest.fan_status
        context['curtain'] = latest.curtain_status
        context['power'] = latest.pow_status
        context['led1'] = latest.ledlight1_status
        context['led2'] = latest.ledlight2_status
        context['led3'] = latest.ledlight3_status
        context['fcled'] = latest.fcled_status
        # print(context)
        return HttpResponse(content_type=json, content=json.dumps(context))



# 报警实时信息（包括微信报警按钮）
def fire(request):
    # if not request.session.get('is_login', None):
    #     return redirect('/login/')
    if request.method == 'GET':
        context = {}
        alarm_info = models.AlarminfoTb.objects.filter(event_handle_status=0)
        if len(alarm_info):
            latest = alarm_info[0]
            context['sensor_id'] = latest.alarmsensor_id
            context['alarm_type'] = latest.alarmevent_type
            # 发送到微信的警报
            if latest.alarmevent_type == 1:
                context['alarm'] = True
                context['guard'] = False
            else:
                context['alarm'] = False
                context['guard'] = True
            # 将警报事件处理状态置1
            alarm_info.update(event_handle_status=1)
            print(context)
            return HttpResponse(content_type=json, content=json.dumps(context))
        else:
            print("------OK-------")
            return JsonResponse({'res': 1})


# 微信控制器实时数据
def wx_exe(request):
    if request.method == 'GET':
        context = {}
        # status_feedback = models.StatusfeedbackTb.objects.order_by('-wea_date')
        status_feedback = models.StatusfeedbackTb.objects.all()
        latest = status_feedback[0]
        context['fan'] = latest.fan_status
        context['curtain'] = latest.curtain_status
        context['power'] = latest.pow_status
        context['led1'] = latest.ledlight1_status
        context['led2'] = latest.ledlight2_status
        context['led3'] = latest.ledlight3_status
        context['fcled'] = latest.fcled_status
        for key in context.keys():
            if context[key] == 0:
                context[key] == False
            else:
                context[key] == True
        print(context)
        return HttpResponse(content_type=json, content=json.dumps(context))




# # 报警器实时数据
# def fire(request):
#     if not request.session.get('is_login', None):
#         return redirect('/login/')
#     if request.method == 'GET':
#         context = {}
#




