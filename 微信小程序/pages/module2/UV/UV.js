// pages/module2/UV/UV.js
Page({
  back: function () {
    wx.navigateTo({
      url: '../../index/index'
    })
  },

  /**
   * 页面的初始数据
   */
  data: {
    UV:0,
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
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
        var ray = res.data.ray;
        those.setData({
          'UV': ray,
        })
      },
      fail: function (res) {
        console.log(res)
      },
      complete: function (res) {
        console.log(res)
      }
    })
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
        var ray = res.data.ray;
        those.setData({
          'UV': ray,
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