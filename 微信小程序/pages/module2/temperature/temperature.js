// pages/module2/temperature/temperature.js
Page({
  data:{
    tem:0,
    envtem:0,
    hum:0,
    envhum:0,  
  },
  back:function(){
    wx.navigateTo({
      url: '../../index/index'
    })
  },

  /**
   * 页面的初始数据
   */
  data: {
  
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
      //getApp().globalData.Interval3 = setInterval(function () {
      var that = this;
      that.setData({
        tem: 0,
        hum: 0,
        envtem: 0,
        envhum: 0
      })
      wx.request({
        url: 'http://134.175.40.47/current/present',
        method: 'GET',
        data: {
        },
        header: {
          'content-type': 'application/json'
        },
        success: function (res) {
          var those = that;
          console.log(res.data);
          var temperature1 = res.data.tem;
          var dorp1 = res.data.hum;
          var temperature2 = res.data.envtem;
          var drop2 = res.data.envhum;
          those.setData({
            'tem': temperature1,
            'hum': dorp1,
            'envtem': temperature2,
            'envhum':drop2
          })
        },
        fail: function (res) {
          console.log(res)
        },
        complete: function (res) {
          console.log(res)
          
        }
      })
    //}, 2000);

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {
  
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
  
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {
  
  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {
  
  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {
    var that = this;
    wx.request({
      url: 'http://134.175.40.47/current/present',
      method: 'GET',
      data: {
      },
      header: {
        'content-type': 'application/json'
      },
      success: function (res) {
        var those = that;
        console.log(res.data);
        var temperature1 = res.data.tem;
        var dorp1 = res.data.hum;
        var temperature2 = res.data.envtem;
        var drop2 = res.data.envhum;
        those.setData({
          'tem': temperature1,
          'hum': dorp1,
          'envtem': temperature2,
          'envhum': drop2
        })
      },
      fail: function (res) {
        console.log(res)
      },
      complete: function (res) {
        console.log(res)

      }
    }),
      wx.showToast({
        title: '刷新成功',//提示信息
        icon: 'success',//成功显示图标
        duration: 1000//时间
      })
  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {
  
  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {
  
  }
})