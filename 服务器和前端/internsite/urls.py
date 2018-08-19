"""internsite URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.11/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url, include
from django.contrib import admin
from login import views

urlpatterns = [
    # 后台管理
    url(r'^admin/', admin.site.urls),
    # 主页+登录
    url(r'^', include('login.urls')),
    # 登录
    url(r'^login/', include('login.urls')),
    # 下发执行器命令
    url(r'^performer/', include('performer.urls')),
    # 历史记录展示
    url(r'^history/', include('history.urls')),
    # 历史记录查询
    url(r'^search/', include('history.urls')),
    # 实时数据
    url(r'^current/', include('real_time.urls')),
    # 安防记录查询
    url(r'^guard/', include('history.urls')),
    # 微信小程序主目录


]
