//index.js
//获取应用实例
const app = getApp()

Page({
  switch : function () {
    wx.navigateTo({
      url: '../module1/module1'
    })
  },
  weather: function () {
    wx.switchTab({
      url: '../module2/temperature/temperature'
    })
  },
  warning: function () {
    wx.navigateTo({
      url: '../module3/module3'
    })
  },
  about:function(){
    wx.navigateTo({
      url: '../module4/module4',
    })
  },
  backlogin: function () {
    wx.navigateTo({
      url: '../login/login'
    })
  },




  data: {
  },

  //下拉事件
  onPullDownRefresh: function () {
    wx.showToast({
      title: '刷新成功',//提示信息
      icon: 'success',//成功显示图标
      duration: 2000//时间
    })
  },

  //用户转发
  onShareAppMessage: function () {
    return {
      title: '谁是最牛逼的人？',
      path: '/page/user?id=123'
    }
  },

  //事件处理函数
  bindViewTap: function() {
    wx.navigateTo({
      url: '../logs/logs'
    })
  },
  onLoad: function () {
    if (app.globalData.userInfo) {
      this.setData({
        userInfo: app.globalData.userInfo,
        hasUserInfo: true
      })
    } 
  },
})
