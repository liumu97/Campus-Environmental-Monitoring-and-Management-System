# This is an auto-generated Django model module.
# You'll have to do the following manually to clean this up:
#   * Rearrange models' order
#   * Make sure each model has one field with primary_key=True
#   * Make sure each ForeignKey has `on_delete` set to the desired behavior.
#   * Remove `managed = False` lines if you wish to allow Django to create, modify, and delete the table
# Feel free to rename the models, but don't rename db_table values or field names.
from __future__ import unicode_literals

from django.db import models


class AlarmdetailTb(models.Model):
    alarmsensor_id = models.IntegerField(primary_key=True)
    alarm_sensor_name = models.CharField(max_length=255)

    class Meta:
        managed = False
        db_table = 'alarmdetail_tb'


class AlarminfoTb(models.Model):
    alarm_time = models.DateTimeField(primary_key=True)
    alarmevent_type = models.SmallIntegerField(blank=True, null=True)
    alarmsensor_id = models.SmallIntegerField(blank=True, null=True)
    event_handle_status = models.SmallIntegerField(blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'alarminfo_tb'


class AuthorityrecordTb(models.Model):
    login_id = models.CharField(primary_key=True, max_length=255)
    logpassword = models.CharField(max_length=255)
    authority = models.SmallIntegerField()
    user_name = models.CharField(max_length=255)
    schoolcard_num = models.CharField(max_length=255)
    car_num = models.CharField(max_length=255, blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'authorityrecord_tb'


class DjangoMigrations(models.Model):
    app = models.CharField(max_length=255)
    name = models.CharField(max_length=255)
    applied = models.DateTimeField()

    class Meta:
        managed = False
        db_table = 'django_migrations'


class ParkingRecordTb(models.Model):
    parkingcard_id = models.CharField(max_length=255)
    arrive_time = models.DateTimeField(primary_key=True)
    leave_time = models.DateTimeField(blank=True, null=True)
    operation = models.SmallIntegerField()

    class Meta:
        managed = False
        db_table = 'parking_record_tb'


class RoomEnvdataTb(models.Model):
    env_date = models.DateTimeField(primary_key=True)
    indoor_env_tem = models.FloatField(blank=True, null=True)
    indoor_env_hum = models.FloatField(blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'room_envdata_tb'


class SecurityBlogTb(models.Model):
    access_date = models.DateTimeField(primary_key=True)
    access_cardid = models.CharField(max_length=255)
    is_correct = models.SmallIntegerField()

    class Meta:
        managed = False
        db_table = 'security_blog_tb'


class StatusfeedbackTb(models.Model):
    status_num = models.IntegerField(primary_key=True)
    fcled_status = models.SmallIntegerField(blank=True, null=True)
    ledlight1_status = models.SmallIntegerField(blank=True, null=True)
    ledlight2_status = models.SmallIntegerField(blank=True, null=True)
    ledlight3_status = models.SmallIntegerField(blank=True, null=True)
    alarmlight_status = models.SmallIntegerField(blank=True, null=True)
    alarmbell_status = models.SmallIntegerField(blank=True, null=True)
    curtain_status = models.SmallIntegerField(blank=True, null=True)
    fan_status = models.SmallIntegerField(blank=True, null=True)
    pow_status = models.SmallIntegerField(blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'statusfeedback_tb'


class WeatherData2Tb(models.Model):
    wea_date2 = models.DateTimeField(primary_key=True)
    pm_data = models.FloatField(blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'weather_data2_tb'


class WeatherDataTb(models.Model):
    wea_date1 = models.DateTimeField(primary_key=True)
    light = models.FloatField(blank=True, null=True)
    co2 = models.IntegerField(blank=True, null=True)
    outdoor_tem_data = models.FloatField(blank=True, null=True)
    outdoor_hum_data = models.FloatField(blank=True, null=True)
    ultra_ray = models.IntegerField(blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'weather_data_tb'

    def __str__(self):
        return str(self.wea_date1)