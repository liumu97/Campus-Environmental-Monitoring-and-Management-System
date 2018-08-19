from django.conf.urls import url
from history import views

urlpatterns = [
    # 历史数据图表
    url(r'^$', views.charts),
    # 按传感器查询
    url(r'^sensor/$', views.sensor),
    # 按系统类型查询
    url(r'^type/$', views.system),
    # 安防系统查询
    url(r'^security/$', views.security),
]

