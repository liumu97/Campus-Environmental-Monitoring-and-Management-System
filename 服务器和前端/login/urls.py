from django.conf.urls import url, include
from django.contrib import admin
from login import views


urlpatterns = [
    url(r'^$', views.login),
    url(r'^index/$', views.index),
    url(r'^logout/$', views.logout),     # 注销页面
    url(r'^wx_login', views.wx_login),   # 微信登录
    # url(r'^send/', views.control, name='send_control'),

]