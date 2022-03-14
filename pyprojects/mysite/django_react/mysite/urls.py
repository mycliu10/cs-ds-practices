from django.contrib import admin
from django.urls import include, path

urlpatterns = [
    path('', include('frontend.urls')),
    path('', include('grocery_api.urls')),
    path('weather/', include('weather.urls')),
    path('polls/', include('polls.urls')),
    path('admin/', admin.site.urls),
]



