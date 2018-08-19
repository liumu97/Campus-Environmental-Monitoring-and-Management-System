Page({
  data: {
    infoMess: '',
    userName: '',
    userN: '',
    passWd: '',
    passW: '',

  },
  //用户名和密码输入框事件
  userNameInput: function (e) {
    this.setData({
      userN: e.detail.value
    })
  },
  passWdInput: function (e) {
    this.setData({
      passW: e.detail.value
    })
  },

  //登录按钮点击事件，调用参数要用：this.data.参数；
  //设置参数值，要使用this.setData({}）方法
  loginBtnClick: function () {
    var userName = this.data.userN;
    var passwords = this.data.passW;
    var that = this;
    if (userName.length == 0 || passwords.length == 0) {
      wx.showToast({
        title: '信息不能为空',
        icon: 'loading',
        duration: 1000
      })
    }
    else {
      // var urlStr = app.globalData.BaseURL + '/user/iosLogin';
      wx.request({
        method: "POST",
        // url: 'http://192.168.43.160:8081/login/wx_login/',
        url:'http://134.175.40.47/login/wx_login',
        data: {
          username: userName,
          password: passwords
        },
        header: {
          "Content-Type": "application/x-www-form-urlencoded; charset=UTF-8"
        },
        success: function (res) {
          console.log(res)
          var PW= res.data.res
          if (PW == '11') {
            wx.navigateTo({
              url: "../index/index"
            });
            wx.showToast({
              title: '登陆成功',
              icon: 'success',
              duration: 1000
            })
          } else {
            wx.showToast({
              title: '失败',
              icon: 'loading',
              duration: 1000
            })
          }
        },
        fail: function () {
          console.log("登录失败");
          wx.showToast({
            title: '密码错误',
            icon: 'loading',
            duration: 2000
          })
        }
      })
    }

  },
  //重置按钮点击事件
  resetBtnClick: function (e) {
    this.setData({
      infoMess: '',
      userName: '',
      userN: '',
      passWd: '',
      passW: '',
    })
  },

  buttonTap: function () {
    wx.navigateTo({
      url: '../index/index',
      success: console.log("页面跳转成功"),
    })
  },

})
