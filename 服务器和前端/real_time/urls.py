from django.conf.urls import url
from real_time import views

urlpatterns = [
    url(r'^present/$', views.present),   # 实时展示的传感器数据

    url(r'^execute/$', views.execute),   # 返回执行器的状态

    url(r'^fire/$', views.fire),         # 返回安防、消防系统的实时状态

    url(r'^wx_exe/$', views.wx_exe),     # 返回小程序的执行器状态
    # 微信数据-天气
    # url(r'^1/$', ),
    # url(r'^2/$', ),
    # url(r'^3/$', ),
    # url(r'^4/$', ),
    # 微信数据-报警系统
    # url(r'^5/$', ),
    # 微信数据-控制器
    # url(r'^6/$', ),
]