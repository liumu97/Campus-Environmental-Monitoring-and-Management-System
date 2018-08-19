from django.conf.urls import url
from performer import views

urlpatterns = [
    url(r'^switch/$', views.switch),         # 开关类控制
    url(r'^light/$', views.light),          # 彩灯控制
    url(r'^gate/$', views.gate),       # 门类控制
    url(r'^lights/', views.lights),     # 照明灯控制
    url(r'^alarm/$', views.alarm),      #警报解除
    url(r'^wx_control/$', views.wx_control),    # 微信普通控制
    url(r'^wx_alarm/$', views.wx_alarm),        # 微信报警器控制
    url(r'^fingerprint/$', views.fingerprint),  # 指纹相关操作
    # wx-控制系统
    # url(r'^1/$', ),
    # wx-报警灯控制
    # url(r'^2/$', ),

]
