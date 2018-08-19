// pages/module3/module3.js
Page({
  onReady: function (e) {
    // 使用 wx.createAudioContext 获取 audio 上下文 context
    this.audioCtx = wx.createAudioContext('myAudio')
  },


  data: {


    Interval:0,
    Interval2: 0

  },
  



  switch1Change: function(e) {
    console.log('switch1 发生 change 事件，携带值为', e.detail.value)
    var start1 = e.detail.value;
    if (start1 == true) {
      getApp().globalData.Interval1 = setInterval(function() {
        var that = this;
        wx.vibrateLong({
          success: console.log('发出振动')
        })
      }, 800);
    }
    if(start1 == false){
      var that = this;
      clearInterval(getApp().globalData.Interval1);
      console.log('关闭警报')
    }
    // if (start1 == true) {
    //   anfang: 1
    // }
    // if (e.detail.valuev == false) {
    //   anfang: 0
    // }
    wx.request({
      url: 'http://192.168.43.160:8081/performer/wx_alarm/',
      method: 'POST',
      data: {
        button_type: 2,
        on_off: e.detail.value
      },
      header: {
        "Content-Type": "application/x-www-form-urlencoded"
      },
      success: function (res) {
        console.log(res.data);
      },
      fail: function (res) {
        console.log(res)
      },
      complete: function (res) {
        console.log(res)
      }
    })
  },

  switch2Change: function(e) {
    console.log('switch2 发生 change 事件，携带值为', e.detail.value)
    var start2 = e.detail.value;
    if (start2 == true) {
      getApp().globalData.Interval2 = setInterval(function () {
        var that = this;
        wx.vibrateLong({
          success: console.log('发出振动')
        })
      }, 800);
    }
    if (start2 == false) {
      var that = this;
      clearInterval(getApp().globalData.Interval2);
      console.log('关闭警报')
    }
    // if (start2 == true) {
    //   xiaofang: 1
    // }
    // if (e.detail.valuev == false) {
    //   xiaofang: 0
    // }
    wx.request({
      url: 'http://192.168.43.160:8081/performer/wx_alarm/',
      method: 'POST',
      data: {
        button_type: 1,
        on_off: e.detail.value
      },
      header: {
        "Content-Type": "application/x-www-form-urlencoded"
      },
      success: function (res) {
        console.log(res.data);
      },
      fail: function (res) {
        console.log(res)
      },
      complete: function (res) {
        console.log(res)
      }
    })
  },

  anfangClick:function(){
    var that = this;
    clearInterval(getApp().globalData.Interval1);
    console.log('关闭安防警报')
  },
  xiaofangClick:function(){
    var that = this;
    clearInterval(getApp().globalData.Interval1);
    console.log('关闭消防警报')
  },


  /**
   * 页面的初始数据
   */
  data: {
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function(options) {

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function() {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function() {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function() {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function() {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function() {
    var that = this;
    wx.request({
      url: 'http://192.168.43.160:8081/current/fire/',
      method: 'GET',
      data: {
      },
      header: {
        'content-type': 'application/json'
      },
      success: function (res) {
        var those = that;
        console.log(res.data);
        var guard = res.data.guard;
        var alarm = res.data.alarm;
        console.log(res.data.guard);
        console.log(res.data.alarm);
        // those.setData({
        //   guard: guard,
        //   alarm: alarm,
        // })
        // if (those.data.guard == true){
        //   getApp().globalData.Interval1 = setInterval(function () {
        //     var that = this;
        //     wx.vibrateLong({
        //       success: console.log('发出振动')
        //     })
        //   }, 800);
        // }
        // if(those.data.alarm == true){
        //   getApp().globalData.Interval1 = setInterval(function () {
        //     var that = this;
        //     wx.vibrateLong({
        //       success: console.log('发出振动')
        //     })
        //   }, 800);
        // }
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
  onReachBottom: function() {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function() {

  }
})