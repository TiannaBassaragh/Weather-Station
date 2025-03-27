 #!/usr/bin/python3


#################################################################################################################################################
#                                                    CLASSES CONTAINING ALL THE APP FUNCTIONS                                                                                                    #
#################################################################################################################################################


class DB:

    def __init__(self,Config):

        from math import floor
        from os import getcwd
        from os.path import join
        from json import loads, dumps, dump
        from datetime import timedelta, datetime, timezone 
        from pymongo import MongoClient , errors, ReturnDocument
        from urllib import parse
        from urllib.request import  urlopen 
        from bson.objectid import ObjectId  
       
      
        self.Config                         = Config
        self.getcwd                         = getcwd
        self.join                           = join 
        self.floor                      	= floor 
        self.loads                      	= loads
        self.dumps                      	= dumps
        self.dump                       	= dump  
        self.datetime                       = datetime
        self.ObjectId                       = ObjectId 
        self.server			                = Config.DB_SERVER
        self.port			                = Config.DB_PORT
        self.username                   	= parse.quote_plus(Config.DB_USERNAME)
        self.password                   	= parse.quote_plus(Config.DB_PASSWORD)
        self.remoteMongo                	= MongoClient
        self.ReturnDocument                 = ReturnDocument
        self.PyMongoError               	= errors.PyMongoError
        self.BulkWriteError             	= errors.BulkWriteError  
        self.tls                            = False # MUST SET TO TRUE IN PRODUCTION


    def __del__(self):
            # Delete class instance to free resources
            pass
 


    ####################
    # DATABASE UTIL FUNCTIONS#
    ####################
    
    def update(self,data):
        '''Insert data into weather station collection'''
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = remotedb.ELET2415.weatherstation.insert_one(data)
        except Exception as e:
            msg = str(e)
            if "duplicate" not in msg:
                print("update error ",msg)        
            return False
        else:                  
            return True
        
       

    def getAllInRange(self,start, end):
        '''Retrieve all records within a timestamp range'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            query       = {"timestamp":{"$gte":int(start),"$lte":int(end)}}, {"_id":0}
            result      = list(remotedb.ELET2415.weatherstation.find(query).sort('timestamp', 1))
        except Exception as e:
            msg = str(e)
            print("getAllInRange error ",msg)
            return []    
        else:                  
            return result
        

    # def calculateMMAR(self, field, start, end):
    #     """Generic function to calculate Min, Max, Avg, Range for a given field within the given date range"""
    #     try:
    #         start = int(start)
    #         end = int(end)
    #         remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
    #         result      = list(remotedb.ELET2415.weatherstation.aggregate([
    #             {'$match': {
    #                 'timestamp': {
    #                     '$gte': start, '$lte': end
    #                 }
    #             }}, 
    #             {'$group': {
    #                 '_id': field,
    #                 field : {
    #                     '$push': "$$ROOT.field"
    #                 }
    #             }},
    #             {'$project': {
    #                 'max': {'$max': f'${field}'},
    #                 'min': {'$min': f'${field}'},
    #                 'avg': {'$avg': f'${field}'},
    #                 'range': {
    #                     '$subtract': [
    #                         {'$max': f'${field}'}, 
    #                         {'$min': f'${field}'}
    #                     ]
    #                 }
    #             }}
    #         ]))
            
    #     except Exception as e:
    #         print(f"{field}MMAR error ", str(e))
    #         return []         
    #     else:                  
    #         return result


    def humidityMMAR(self,start, end):
        '''Calculates Min, Max, Avg, Range for humidity within the given date range'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weatherstation.aggregate( [
                {'$match': {
                    'timestamp': {
                        '$gte': start, 
                        '$lte': end
                    }
                }}, 
                {'$group': {
                    '_id': 'humidity', 
                    'humidity': {
                        '$push': "$$ROOT.humidity"
                    }
                }}, 
                {'$project': {
                    'max': {'$max': '$humidity'}, 
                    'min': {'$min': '$humidity'}, 
                    'avg': {'$avg': '$humidity'}, 
                    'range': {
                        '$subtract': [
                            {'$max': '$humidity'}, 
                            {'$min': '$humidity'}
                        ]
                    }
                }}
            ]))
        except Exception as e:
            msg = str(e)
            print("humidityMMAR error ",msg)
            return []
        else:                  
            return result

    def temperatureMMAR(self,start, end):
        '''Calculates Min, Max, Avg, Range for temperature within the given date range'''
        try:
            start=int(start)
            end=int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weatherstation.aggregate( [
                {'$match': {
                    'timestamp': {
                        '$gte': start, 
                        '$lte': end
                    }
                }}, 
                {'$group': {
                    '_id': 'temperature', 
                    'temperature': {
                        '$push': "$$ROOT.temperature"
                    }
                }}, 
                {'$project': {
                    'max': {'$max': '$temperature'}, 
                    'min': {'$min': '$temperature'}, 
                    'avg': {'$avg': '$temperature'}, 
                    'range': {
                        '$subtract': [
                            {'$max': '$temperature'}, 
                            {'$min': '$temperature'}
                        ]
                    }
                }}
            ]))
        except Exception as e:
            msg = str(e)
            print("temperatureMMAR error ",msg)
            return []
        else:                  
            return result

    def pressureMMAR(self,start, end):
        '''Calculates Min, Max, Avg, Range for pressure within the given date range'''
        try:
            start=int(start)
            end=int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weatherstation.aggregate( [
                {'$match': {
                    'timestamp': {
                        '$gte': start, 
                        '$lte': end
                    }
                }}, 
                {'$group': {
                    '_id': 'pressure', 
                    'pressure': {
                        '$push': "$$ROOT.pressure"
                    }
                }}, 
                {'$project': {
                    'max': {'$max': '$pressure'}, 
                    'min': {'$min': '$pressure'}, 
                    'avg': {'$avg': '$pressure'}, 
                    'range': {
                        '$subtract': [
                            {'$max': '$pressure'}, 
                            {'$min': '$pressure'}
                        ]
                    }
                }}
            ]))
        except Exception as e:
            msg = str(e)
            print("pressureMMAR error ",msg)
            return []
        else:                  
            return result

    def moistureMMAR(self,start, end):
        '''Calculates Min, Max, Avg, Range for moisture level within the given date range'''
        try:
            start=int(start)
            end=int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weatherstation.aggregate( [
                {'$match': {
                    'timestamp': {
                        '$gte': start, 
                        '$lte': end
                    }
                }}, 
                {'$group': {
                    '_id': 'soilmoisturePercent', 
                    'soilmoisturePercent': {
                        '$push': "$$ROOT.soilmoisturePercent"
                    }
                }}, 
                {'$project': {
                    'max': {'$max': '$soilmoisturePercent'}, 
                    'min': {'$min': '$soilmoisturePercent'}, 
                    'avg': {'$avg': '$soilmoisturePercent'}, 
                    'range': {
                        '$subtract': [
                            {'$max': '$soilmoisturePercent'}, 
                            {'$min': '$soilmoisturePercent'}
                        ]
                    }
                }}
            ]))
        except Exception as e:
            msg = str(e)
            print("moistureMMAR error ",msg)
            return []
        else:
            return result

    def frequencyDistro(self,variable,start, end):
        '''Returns frequency distribution for a specified variable within a given range of time'''
        try:
            start=int(start)
            end=int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weatherstation.aggregate( [
                {'$match': {
                    'timestamp': { 
                        '$gte': start, '$lte': end
                    }
                }},
                {'$bucketAuto': {
                    'groupBy': f"${variable}",
                    'buckets': 4,
                    'output': {
                        'count': { '$sum': 1 }
                    }
                }}
            ]))
        except Exception as e:
            msg = str(e)
            print("frequencyDistro error ",msg)
            return []
        else:                  
            return result


def main():
    from config import Config
    from time import time, ctime, sleep
    from math import floor
    from datetime import datetime, timedelta
    one = DB(Config)
 
 
    start = time() 
    end = time()
    print(f"completed in: {end - start} seconds")
    
if __name__ == '__main__':
    main()
