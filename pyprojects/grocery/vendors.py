import os
import sys
currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)

import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import datetime
import requests
import json
from bs4 import BeautifulSoup
import myhelp

class Target:
    _name = "target"

    def __init__(self):
        return

    def search(self, name):
        url = "".join( ("https://redsky.target.com/redsky_aggregations/v1/web/plp_search_v1?key=9f36aeafbe60771e321a7cc95a78140772ab3e96&channel=WEB&count=24&default_purchasability_filter=true&include_sponsored=true&keyword=", name, "&offset=0&page=%2Fs%2F", name, "&platform=desktop&pricing_store_id=666&scheduled_delivery_store_id=1969&store_ids=666%2C2851%2C1969%2C1978%2C1236&useragent=Mozilla%2F5.0+%28Macintosh%3B+Intel+Mac+OS+X+10_15_7%29+AppleWebKit%2F537.36+%28KHTML%2C+like+Gecko%29+Chrome%2F97.0.4692.71+Safari%2F537.36&visitor_id=017F5DCA70A7020185B2202B65A69D6C") )
        
        payload={}
        headers = {
            'authority': 'redsky.target.com',
            'pragma': 'no-cache',
            'cache-control': 'no-cache',
            'sec-ch-ua': '" Not;A Brand";v="99", "Google Chrome";v="97", "Chromium";v="97"',
            'accept': 'application/json',
            'sec-ch-ua-mobile': '?0',
            'user-agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36',
            'sec-ch-ua-platform': '"macOS"',
            'origin': 'https://www.target.com',
            'sec-fetch-site': 'same-site',
            'sec-fetch-mode': 'cors',
            'sec-fetch-dest': 'empty',
            'referer': 'https://www.target.com/s?searchTerm=' + name,
            'accept-language': 'en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7',
            'cookie': 'TealeafAkaSid=Q6POWvahrOdJ1jtxxKgUqX2AWpFsOeLy; visitorId=017F5DCA70A7020185B2202B65A69D6C; sapphire=1; UserLocation=43016|40.060|-83.150|OH|US; fiatsCookie=DSI_666|DSN_Columbus%20NW|DSZ_43017; ci_pixmgr=other; _gcl_au=1.1.1570380276.1646546026; cd_user_id=17f5dca837d970-0704e96f89b4f7-1d326253-1fa400-17f5dca837e7ef; accessToken=eyJraWQiOiJlYXMyIiwiYWxnIjoiUlMyNTYifQ.eyJzdWIiOiI2MDZmYTM3Ni1hODFkLTQ4MzUtYjVkYS1kMmRjOGM1ZmMxZmQiLCJpc3MiOiJNSTYiLCJleHAiOjE2NDcyNzYyNDYsImlhdCI6MTY0NzE4OTg0NiwianRpIjoiVEdULmM0MjM0Y2EyM2FmYjQwMTQ5MmExZjZhOGU5NDNmMDZiLWwiLCJza3kiOiJlYXMyIiwic3V0IjoiRyIsImRpZCI6IjdjYWJjMTQyYWNmZGI5NTUyNmYzOGUyNmNmMjMzMzRhYjVmOWI1NTQ1OTZlMDk0NGQ2OTNlMzAxOTAyNmZiZGMiLCJzY28iOiJlY29tLm5vbmUsb3BlbmlkIiwiY2xpIjoiZWNvbS13ZWItMS4wLjAiLCJhc2wiOiJMIn0.Sxv6qxFnlNf3Nus-AS7OCuHpkBqIPQSXqriqN2_bdiVf0bZKmbHMQtZxJpVmmQfq6q7MJqUF0_oSuv15IaLqDDdyHkgsb4lBtT_w7XYInscn8jkAfcRpbUC7OiW1O3AV84Z4xcrN3zJLIwwiTXUgQJAklqFNUH9fiFPuFSBdxmzr0WEVA530LZ8AFQzVrBaBjnu2ribyxl1-hnb-Rgk1CUFAwn73JhKug_vqog04ija8hyqNg7BkyahDIqQTIp2jsZqqpOSjuollBskdnxPndXcrfPrMK0K9hoI9nTWw3mBXnKFd8ibDdG4rGXcfbhVa-uqjYBWN-A7_Qfeea5-kzA; idToken=eyJhbGciOiJub25lIn0.eyJzdWIiOiI2MDZmYTM3Ni1hODFkLTQ4MzUtYjVkYS1kMmRjOGM1ZmMxZmQiLCJpc3MiOiJNSTYiLCJleHAiOjE2NDcyNzYyNDYsImlhdCI6MTY0NzE4OTg0NiwiYXNzIjoiTCIsInN1dCI6IkciLCJjbGkiOiJlY29tLXdlYi0xLjAuMCIsInBybyI6eyJmbiI6bnVsbCwiZW0iOm51bGwsInBoIjpmYWxzZSwibGVkIjpudWxsLCJsdHkiOmZhbHNlfX0.; refreshToken=zzVdJDI26pxe5uvWhPqyVWkVkwpp7nux8Du9FdqorIW2eaPiN7lZlcx5Zwtr26_aFnxtXBLGB1XX7ubYu-oGpA; ffsession={%22sessionHash%22:%22421daa10b1a321646546023853%22%2C%22prevPageName%22:%22search:%20search%20results%22%2C%22prevPageType%22:%22search:%20search%20results%22%2C%22prevPageUrl%22:%22https://www.target.com/s?searchTerm=banana%22%2C%22sessionHit%22:9%2C%22prevSearchTerm%22:%22banana%22}; _uetsid=cea40140a2ec11ecafd9850fcfeff56a; _uetvid=c95aad609d1111ec87bb092837a2609f; _mitata=OTg2MjkyMjIyNDZiNTdhZWY4NzlmODYxYWQ4ZDk4NjA1NmMwODgzMzY0Mjc2MTRmNGYxODBiNTgzYTEzZDVjYw==_/@#/1647238302_/@#/ceKU7ucfNVBHj7G9_/@#/OTQ2MzM1ZWVkZmY0NDljN2UwMmYyNmNhNmFjMjA5ZDE2NjA0NGM3NmNlZmE5OTVmNGFkOThhZTFhZWIzN2Y1Nw==_/@#/000; _mitata=ODVhZmIxOTU0NTZlZTI2MTgxYzJjMGI1MzcxM2E5YTA5NGNmMzVmMjM2MTViZTMwYWUxMDYzYjExOGRjZDMyMQ==_/@#/1647238594_/@#/ceKU7ucfNVBHj7G9_/@#/MmEyZGNmMGEzOWQ2YTgxMjY2NjZjZWI0MjU2ODZkYWYwOTk4Njk1OWFjOTBlY2VmOGQyMmYyMTVjMzQ2ZmYxZQ==_/@#/000'
        }
        
        try:
            response = requests.request("GET", url, headers=headers, data=payload)
        
            rj = response.json()

            result = rj['data']['search']['products']

            result = [ myhelp.extractDict(item, 
                    [{'name': ['item', 'product_description', 'title']}, 
                     {'currentPrice': ['price', 'current_retail']},
                     {'unitPrice': ['price', 'current_retail']} ])
                     for item in result ]
        except:
            result = None

        return result 
    
    
class Walmart:
    _name = "walmart"    
    def __init__(self):
        return

    def search(self, name):
        with open(os.path.join(currentdir,"walmart_query.txt"), "r") as f:
            query = f.read()

        url = "https://www.walmart.com/orchestra/home/graphql/search?query=pasta%20sauce&page=1&prg=desktop&sort=best_match&ps=40&fetchMarquee=true&fetchSkyline=true&fetchSbaTop=true"
        
        payload = json.dumps({
            "query": query,
            "variables": {
                "id": "",
                "dealsId": "",
                "query": name,
                "page": 1,
                "prg": "desktop",
                "catId": "",
                "facet": "",
                "sort": "best_match",
                "rawFacet": "",
                "seoPath": "",
                "ps": 40,
                "ptss": "",
                "trsp": "",
                "beShelfId": "",
                "recall_set": "",
                "module_search": "",
                "min_price": "",
                "max_price": "",
                "storeSlotBooked": "",
                "additionalQueryParams": None,
                "fitmentFieldParams": None,
                "fitmentSearchParams": {
                    "id": "",
                    "dealsId": "",
                    "query": name,
                    "page": 1,
                    "prg": "desktop",
                    "catId": "",
                    "facet": "",
                    "sort": "best_match",
                    "rawFacet": "",
                    "seoPath": "",
                    "ps": 40,
                    "ptss": "",
                    "trsp": "",
                    "beShelfId": "",
                    "recall_set": "",
                    "module_search": "",
                    "min_price": "",
                    "max_price": "",
                    "storeSlotBooked": "",
                    "additionalQueryParams": None,
                    "cat_id": "",
                    "_be_shelf_id": ""
                },
                "fetchMarquee": True,
                "fetchSkyline": True,
                "fetchSbaTop": True
            }
        })
        headers = {
            'sec-ch-ua': '" Not;A Brand";v="99", "Google Chrome";v="97", "Chromium";v="97"',
            'x-o-platform': 'rweb',
            'x-o-correlation-id': '0z4am1YDb2B12G3fB1iOVtI-PATED3vVBCBL',
            'DEVICE_PROFILE_REF_ID': 'fmEjPY4jJ21lQqdQpc1c0PQ0xQT0Mo6jzQUz',
            'x-latency-trace': '1',
            'WM_MP': 'true',
            'x-o-market': 'us',
            'x-o-platform-version': 'main-347-5e3156',
            'x-o-gql-query': 'query Search',
            'WM_PAGE_URL': 'https://www.walmart.com/search?q=pasta%20sauce&typeahead=pasta',
            'X-APOLLO-OPERATION-NAME': 'Search',
            'sec-ch-ua-platform': '"macOS"',
            'sec-ch-ua-mobile': '?0',
            'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36',
            'x-o-segment': 'oaoh',
            'Content-Type': 'application/json',
            'accept': 'application/json',
            'Referer': 'https://www.walmart.com/search?q=pasta%20sauce&typeahead=pasta',
            'x-enable-server-timing': '1',
            'x-o-ccm': 'server',
            'wm_qos.correlation_id': '0z4am1YDb2B12G3fB1iOVtI-PATED3vVBCBL',
            'Cookie': '47'
        }

        try:
            response = requests.request("POST", url, headers=headers, data=payload)

            rj = response.json()

            result = rj['data']['search']['searchResult']['itemStacks'][0]['itemsV2']
            
            result = [ myhelp.extractDict(item, 
                    ['name', {'currentPrice': ['priceInfo', 'currentPrice', 'price']},
                     {'unitPrice': ['priceInfo', 'unitPrice', 'priceString']}])
                     for item in result ]
        except:
            result = None

        return result



class Vendors:
    _vendors = [Walmart(), Target()]

    def __init__(self):
        return


    def getPrice(self, name):
        result = {}
        for v in self._vendors:
            result[v._name] = v.search(name)

        return result


    def checkPrice(self, name):
        for v in self._vendors:
            print( f"from {v._name}:" )
            r = v.search(name)
            for i in r:
                print( f"{i['name']}: price $ {i['currentPrice']} , unit price $ {i['unitPrice']}")


