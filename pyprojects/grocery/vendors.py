import sys
sys.path.append("../")
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
            'cookie': 'TealeafAkaSid=Q6POWvahrOdJ1jtxxKgUqX2AWpFsOeLy; visitorId=017F5DCA70A7020185B2202B65A69D6C; sapphire=1; UserLocation=43016|40.060|-83.150|OH|US; egsSessionId=31f25626-ca08-41d9-ae79-6d2c64fdfa21; accessToken=eyJraWQiOiJlYXMyIiwiYWxnIjoiUlMyNTYifQ.eyJzdWIiOiI2MDZmYTM3Ni1hODFkLTQ4MzUtYjVkYS1kMmRjOGM1ZmMxZmQiLCJpc3MiOiJNSTYiLCJleHAiOjE2NDY2MzI0MjQsImlhdCI6MTY0NjU0NjAyNCwianRpIjoiVEdULjFjNDFhMTJmYmZjZTQ3Y2NhNmQ0ZDA2NjBkMDdjNTVlLWwiLCJza3kiOiJlYXMyIiwic3V0IjoiRyIsImRpZCI6IjdjYWJjMTQyYWNmZGI5NTUyNmYzOGUyNmNmMjMzMzRhYjVmOWI1NTQ1OTZlMDk0NGQ2OTNlMzAxOTAyNmZiZGMiLCJzY28iOiJlY29tLm5vbmUsb3BlbmlkIiwiY2xpIjoiZWNvbS13ZWItMS4wLjAiLCJhc2wiOiJMIn0.rW7ou_mUeTBlQKl2hSvt8vyxgqOa7NgDbop1YuiWSfutaEsQs5zl5LEXDErIFZ7jd9s7pu-8S-JdwTaIhzS7CzOFo_qBP9HjpBamq648qTyrOoqrv-WEWUyF0-P6jUuAolY8nF7jT5COCUXJ30bR23lrYoHy-p-oDNPGaFD1jZGk5BBVyPUyKObpWmjFv-kqoxkSuwDug4wjcU7viYNOGjgusIEoyDQ9lTUF19sqF7tVnm35hgr95LhpDz8UhY6VRFadN_JZAE4u7u3yJv1V0GONEUNNhfAHaQCB8M4fSk6gXlc-9ifyPlE1ORh4q6OTJMvWxrWguLcB5_3UnT16Dw; idToken=eyJhbGciOiJub25lIn0.eyJzdWIiOiI2MDZmYTM3Ni1hODFkLTQ4MzUtYjVkYS1kMmRjOGM1ZmMxZmQiLCJpc3MiOiJNSTYiLCJleHAiOjE2NDY2MzI0MjQsImlhdCI6MTY0NjU0NjAyNCwiYXNzIjoiTCIsInN1dCI6IkciLCJjbGkiOiJlY29tLXdlYi0xLjAuMCIsInBybyI6eyJmbiI6bnVsbCwiZW0iOm51bGwsInBoIjpmYWxzZSwibGVkIjpudWxsLCJsdHkiOmZhbHNlfX0.; refreshToken=jmd7KEuusiSQvlFtuAi2gAWeKkguAIzCfeIv5AAc21uU8t4FisJy-8kUX1Qpwp8NO0qVbzydpOyXlxfUppfY1A; fiatsCookie=DSI_666|DSN_Columbus%20NW|DSZ_43017; ci_pixmgr=other; _gcl_au=1.1.1570380276.1646546026; cd_user_id=17f5dca837d970-0704e96f89b4f7-1d326253-1fa400-17f5dca837e7ef; ffsession={%22sessionHash%22:%22421daa10b1a321646546023853%22%2C%22prevPageName%22:%22search:%20search%20results%22%2C%22prevPageType%22:%22search:%20search%20results%22%2C%22prevPageUrl%22:%22https://www.target.com/s?searchTerm=banana%22%2C%22sessionHit%22:4%2C%22prevSearchTerm%22:%22banana%22}; _uetsid=c95a7eb09d1111ecac55e5b08dfaaf36; _uetvid=c95aad609d1111ec87bb092837a2609f; _mitata=YzNlMjY0YWE0Zjk3YzNkOWRkNmNiY2QwNjUyMDQyMWVhMTU2NTU2NmQ2NTc5YmVmMTNmNzY2YjcxYTFkN2NjNA==_/@#/1646546404_/@#/c9EyxkVxPz9Lk2Fp_/@#/MWU1YzY4NWI1NzI2YThiZmFlZjY5MzAwZDQ3ZGQ2ZTljNzllOGYzMTg4MmMzMGE3ZGEzYzRjYzVlNmUzYTFhZg==_/@#/000; _mitata=MmFlNTYwOGNiYjkwYjVjNjlmOTQ2YWZjN2IxYjhkM2I4M2ViYzNlNDRlZWEwM2Q2ZDk0MmViOTE2YWRjNGVhMg==_/@#/1646546679_/@#/c9EyxkVxPz9Lk2Fp_/@#/M2VhZjQwNzZiYjVhYTNlZjM4N2EzMjM5MzM2NTQ5ZjM3NGRmMzU4MWMxODE2Yjg1N2YxOGU4YzNiNWNiOTljZQ==_/@#/000'
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
        with open("walmart_query.txt", "r") as f:
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


    def checkPrice(self, name):
        for v in self._vendors:
            print( f"from {v._name}:" )
            r = v.search(name)
            for i in r:
                print( f"{i['name']}: price $ {i['currentPrice']} , unit price $ {i['unitPrice']}")


