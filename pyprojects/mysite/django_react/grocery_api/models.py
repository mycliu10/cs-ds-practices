from django.db import models


# Create your models here.

class Grocery(models.Model):
    keyword = models.CharField(max_length=64)
    created_time = models.DateTimeField(auto_now_add=True)
    search_results = models.JSONField()




