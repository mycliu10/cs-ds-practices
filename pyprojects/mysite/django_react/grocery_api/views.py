from django.shortcuts import render
from .models import Grocery
from .serializers import GrocerySerializer
from rest_framework import generics, status
from rest_framework.response import Response
from rest_framework.views import APIView

import os
import sys
currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.join(currentdir, "../../../grocery/")
sys.path.append(parentdir)
import vendors


# Create your views here.
class GroceryList(APIView):
    def get(self, request, *args, **kwargs):
        data = request.GET
        keyword = data['keyword']
        if keyword is None:
            return Response({'Bad Request: No Keyword Given.'}, status=status.HTTP_400_BAD_REQUEST)

        search_results = vendors.Vendors().getPrice(keyword)
        return Response(data=search_results)
