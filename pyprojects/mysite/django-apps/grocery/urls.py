from django.urls import path

from . import views


urlpatterns = [
    path('api/grocery/', views.GroceryList.as_view()),
]
