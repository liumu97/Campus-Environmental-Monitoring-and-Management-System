// pages/module1/module1.js
Page({
  data: {
    // text:"这是一个页面"
    array: ['关闭', '模式1', '模式2', '模式3'],
    index: 0,
    text1:0
  },

  listenerPickerSelected: function(e){
    // wx.request({
    //   url: 'http://192.168.43.160:8081/performer/wx_control/',
    //   method: 'POST',
    //   data: {
    //     pack_type: 2,
    //     button_type: index,
    //   },
    //   header: {
    //     "Content-Type": "application/x-www-form-urlencoded"
    //   },
    //   success: function (res) {
    //     console.log(res.data);
    //   },
    //   fail: function (res) {
    //     console.log(res)
    //   },
    //   complete: function (res) {
    //     console.log(res)
    //   }
    // })
  },





  slider1change:function(e){
    console.log('slider1 发生 change 事件，携带值为', e.detail.value)
    wx.request({
      url: 'http://134.175.40.47/performer/wx_control/',
      method: 'POST',
      data: {
        pack_type: 4,
        button_type: 1,
        bright: e.detail.value
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
  slider2change: function (e) {
    console.log('slider2 发生 change 事件，携带值为', e.detail.value)
    wx.request({
      url: 'http://134.175.40.47/performer/wx_control/',
      method: 'POST',
      data: {
        pack_type: 4,
        button_type: 2,
        bright: e.detail.value
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
  slider3change: function (e) {
    console.log('slider3 发生 change 事件，携带值为', e.detail.value)
    wx.request({
      url: 'http://134.175.40.47/performer/wx_control/',
      method: 'POST',
      data: {
        pack_type: 4,
        button_type: 3,
        bright: e.detail.value
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





  // switch0Change: function (e) {
  //   console.log('switch0 发生 change 事件，携带值为', e.detail.value)
  //   this.setData({
  //     picker: !e.detail.value,
  //   })
  // },
  // switch1Change: function (e) {
  //   console.log('switch1 发生 change 事件，携带值为', e.detail.value)
  //   this.setData({
  //     slider1: !e.detail.value,
  //     led1:0
  //   })
  // },
  // switch2Change: function (e) {
  //   console.log('switch2 发生 change 事件，携带值为', e.detail.value)
  //   this.setData({
  //     slider2: !e.detail.value,
  //     led2: 0
  //   })
  // },
  // switch3Change: function (e) {
  //   console.log('switch3 发生 change 事件，携带值为', e.detail.value)
  //   this.setData({
  //     slider3: !e.detail.value,
  //     led3: 0
  //   })
  // },

  switch4Change: function (e) {
    console.log('switch4 发生 change 事件，携带值为', e.detail.value)
    var that = this
    var kaiguanA = e.detail.value
    // if(that.data.kaiguanA == true){
    //   that.setData({
    //     fankaiguan:1
    //   })
    // }
    // if (this.data.kaiguanA == false){
    //   that.setData({
    //     fankaiguan: 0
    //   })
    // }
    wx.request({
      url: 'http://134.175.40.47/performer/wx_control/',
      method: 'POST',
      data: {
        pack_type: 1,
        button_type: 'fan',
        on_off: e.detail.value
      },
      header: {
        "Content-Type": "application/x-www-form-urlencoded"
      },
      success: function (res) {
        console.log(res.data);
        console.log(e.detail.value)
      },
      fail: function (res) {
        console.log(res)
      },
      complete: function (res) {
        console.log(res)
      }
    })
  },
  switch5Change: function (e) {
    console.log('switch5 发生 change 事件，携带值为', e.detail.value)
    // var that = this
    // var kaiguanA = e.detail.value
    // if (that.data.kaiguanB == true) {
    //   this.setData({
    //     curtainkaiguan: 1
    //   })
    // }
    // if (this.data.kaiguanB == false) {
    //   this.setData({
    //     curtainkaiguan: 0
    //   })
    // }
    wx.request({
      url: 'http://134.175.40.47/performer/wx_control/',
      method: 'POST',
      data: {
        pack_type: 1,
        button_type: 'curtain',
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
  switch6Change: function (e) {
    console.log('switch6 发生 change 事件，携带值为', e.detail.value)
    // var that = this
    // var kaiguanA = e.detail.value
    // if (that.data.kaiguanC == true) {
    //   this.setData({
    //     powerkaiguan: 1
    //   })
    // }
    // if (this.data.kaiguanC == false) {
    //   this.setData({
    //     powerkaiguan: 0
    //   })
    // }
    wx.request({
      url: 'http://134.175.40.47/performer/wx_control/',
      method: 'POST',
      data: {
        pack_type: 1,
        button_type: 'power',
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


  parkingClick: function(){
    wx.request({
      url: 'http://134.175.40.47/performer/wx_control/',
      method: 'POST',
      data: {
        pack_type: 3,
        button_type: 'parking',
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

  doorClick: function () {
    wx.request({
      url: 'http://134.175.40.47/performer/wx_control/',
      method: 'POST',
      data: {
        pack_type: 3,
        button_type: 'school',
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














  /**
   * 监听普通picker选择器
   */
  listenerPickerSelected: function(e) {
    console.log('picker 发生 change 事件，携带值为', e.detail.value)
    //改变index值，通过setData()方法重绘界面
    this.setData({
      index: e.detail.value
    });
    wx.request({
      url: 'http://134.175.40.47/performer/wx_control/',
      method: 'POST',
      data: {
        pack_type: 2,
        button_type: e.detail.value,
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



  onLoad: function (options) {
    this.setData({
      picker:true,
      slider1:true,
      slider2: true,
      slider3: true,
    })
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
      url: 'http://134.175.40.47/current/wx_exe/',
      method: 'GET',
      data: {
      },
      header: {
        'content-type': 'application/json'
      },
      success: function (res) {
        var those = that;
        console.log(res.data);
        var caideng = res.data.fcled;
        var deng1 = !res.data.led1;
        var deng2 = !res.data.led2;
        var deng3 = !res.data.led3;
        var fengshan = res.data.fan;
        var lianzi = res.data.curtain;
        var dianyuan = res.data.power;
        those.setData({
          xuancaixiaodeng: caideng,
          zhaomingdeng1: deng1,
          zhaomingdeng2: deng2,
          zhaomingdeng3: deng3,
          kongtiao: fengshan,
          chuanglian: lianzi,
          qiehuandianyuan: dianyuan,
          
        })
        if(those.data.zhaomingdeng1 == true){
          those.setData({
            led1:0
          })
        };
        if (those.data.zhaomingdeng2 == true) {
          those.setData({
            led2: 0
          })
        };
        if (those.data.zhaomingdeng3 == true) {
          those.setData({
            led3: 0
          })
        }
        // if(caideng == true){
        //   picker: false
        // };
        // if (caideng == false) {
        //   picker: true
        // };
        // if(deng1 == true){
        //   slider1: false
        // };
        // if (deng1 == false) {
        //   slider1: true
        // };
        // if (deng2 == true) {
        //   slider2: false
        // };
        // if (deng2 == false) {
        //   slider2: true
        // };
        // if (deng3 == true) {
        //   slider3: false
        // };
        // if (deng3 == false) {
        //   slider3: true
        // };
      },
      fail: function (res) {
        console.log(res)
      },
      complete: function (res) {
        console.log(res);
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

