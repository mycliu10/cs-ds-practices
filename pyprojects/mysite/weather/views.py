from django.shortcuts import render, get_object_or_404

from django.views import generic

import requests
import datetime

# Create your views here.
class IndexView(generic.ListView):
    template_name = 'weather/index.html'
    context_object_name = 'latest_question_list'


class OpenWeatherMap:
    _API_key = "7a378f4e96ee7e3e4590301943c7e2d8"
    _url_current = "http://api.openweathermap.org/data/2.5/weather?q={}&appid={}&units=imperial"
    _url_hourly = "https://api.openweathermap.org/data/2.5/onecall?lat={}&lon={}&appid={}&esclude=current,minutely,daily,alerts&units=imperial"

    def __init__(self):
        return

    def PullCurrent(self, city="dublin,oh,us"):
        url = self._url_current.format(city, self._API_key)
        r = requests.get(url)
        data = r.json()
    
        t = datetime.datetime.fromtimestamp(data['dt']).isoformat(sep='T')  
        temp = data['main']['feels_like']
        w = data['weather'][0]['description']

        return data
    
    def PullHourly(self, lon=-83.1141, lat=40.0992):
        url = self._url_hourly.format(lat.__str__(), lon.__str__(), self._API_key)
        r = requests.get(url)
        data = r.json()
     
        e_list = []
        for d in data['hourly']:
            e = {}
            e['time'] = datetime.datetime.fromtimestamp(d['dt']).isoformat(sep='T') 
            e['feels_like'] = d['feels_like'] 
            e['description'] = d['weather'][0]['description'] 
            e['pop'] = d['pop'] 
            e_list.append( e )
    
        return e_list


class Weatherbit:
    _API_key = "651b8da8a65248788e8e209056e3296e"
    _url_hourly = "https://api.weatherbit.io/v2.0/forecast/hourly?lat={}&lon={}&key={}&units=I&hours=24"

    def __init__(self):
        return

    def PullHourly(self, lon=-83.1141, lat=40.0992):
        url = self._url_hourly.format(lat, lon, self._API_key)
        r = requests.get(url)
        data = r.json()

        e_list = []
        for d in data['data']:
            e = {}
            e['time'] = datetime.datetime.fromtimestamp(d['ts']).isoformat(sep='T') 
            e['feels_like'] = d['app_temp'] 
            e['description'] = d['weather']['description'] 
            e['pop'] = d['pop'] 
            e_list.append( e )
    
        return e_list



class WeatherCV:
    _sources = {
        'OpenWeatherMap': OpenWeatherMap(),
        'Weatherbit': Weatherbit(),
    }

    def __init__(self):
        return


    def PullHourly(self):
        hourly_reports = {key: value.PullHourly() for key,value in self._sources.items()}
        source_names = list(self._sources.keys())
        variable_names = list(hourly_reports[source_names[0]][0].keys())

        now = datetime.datetime.now()
        now = now.replace(minute=0, second=0, microsecond=0)

        pulled_data = []
        for n in range(1, 13):
            now += datetime.timedelta(hours=1)
            now_str = now.isoformat()

            e = {'time': now_str}

            for name, report in hourly_reports.items():
                times = [ hour['time'] for hour in report ]
                if now_str in times:
                    idx = times.index( now_str )
                    for key, value in report[idx].items():
                        if key != 'time':
                            new_key = "".join( (key,"_",name) )
                            e[new_key] = value
                else:
                    for key in variable_names:
                        if key != 'time':
                            new_key = "".join( (key,"_",name) )
                            e[new_key] = None
           
            pulled_data.append( e ) 
        
        num_sources = len(source_names)
        result = {
            'pulled_hourly': pulled_data,
            'num_sources': num_sources,
        }

        return result



    def Get(self):
        return self._collections 


def main(request):
    wcv = WeatherCV()
    context = wcv.PullHourly()

    return render(request, 'weather/index.html', context)


