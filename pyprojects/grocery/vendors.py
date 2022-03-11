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

        url = "https://redsky.target.com/redsky_aggregations/v1/web_platform/product_summary_with_fulfillment_v1?key=9f36aeafbe60771e321a7cc95a78140772ab3e96&tcins=15013944%2C47780315%2C78438139%2C14599316%2C54541751%2C77632333%2C26396441%2C16951592%2C81415000%2C53140802%2C17316062%2C82095690%2C51964661%2C14394368%2C81471842%2C79314666%2C14779713%2C53140800%2C16185593%2C15346395%2C54557726%2C15993444%2C79276537%2C79314685&store_id=666&zip=43016&state=OH&latitude=40.060&longitude=-83.150&scheduled_delivery_store_id=1969&required_store_id=666&has_required_store_id=true"

        payload={}
        headers = {
            'authority': 'redsky.target.com',
            'sec-ch-ua': '" Not;A Brand";v="99", "Google Chrome";v="97", "Chromium";v="97"',
            'accept': 'application/json',
            'sec-ch-ua-mobile': '?0',
            'user-agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36',
            'sec-ch-ua-platform': '"macOS"',
            'origin': 'https://www.target.com',
            'sec-fetch-site': 'same-site',
            'sec-fetch-mode': 'cors',
            'sec-fetch-dest': 'empty',
            'referer': 'https://www.target.com/s?searchTerm=banana',
            'accept-language': 'en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7',
            'cookie': '47'
        }
        
        try:
            response = requests.request("GET", url, headers=headers, data=payload)
        
            rj = response.json()

            result = rj['data']['product_summaries']

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


