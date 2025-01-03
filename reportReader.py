from collections import defaultdict
import pandas as pd
import re
from pathlib import Path
import sys
import numpy as np
import os
from scipy.optimize import curve_fit

# estimate, i couldnt find true max vals...
MAX_VALS = {"BRAM": 100000, "DSP":13000, "FF":4106900, "LUT":1728000, "URAM":1000}

def saveReport(tracksize, timing, out_name):

  with open("../csynth.rpt", 'r') as file:
    data = file.read()

  data = data.split("\n\n")[2]

  header_raw = data.split("\n")[4:6]
  lines_raw = data.split("\n")[7:]



  header = []
  tmp = []
  for h in header_raw:
    htypes = h.split("|")[1:]
    htypes = [h.strip() for h in htypes]
    tmp.append(htypes)
    # print(htypes)

  header = [tmp[0][i] + " " + tmp[1][i] if tmp[0][i] != '' else tmp[1][i] for i in range(len(tmp[0])) ][:-1]
  # for h in header:
  #   print(h)

  # print(header)
  # print(len(header))

  # print("-=-=-=-=-=-=-=\n")

  data = defaultdict(list)

  # full_data = []
  for l in lines_raw[:-1]:
    ldata = l.split("|")[1:]
    ldata = [d.strip() for d in ldata]
    # full_data.append(ldata)
    # data[]
    # print(ldata)
    # print(len(ldata))
    for i, h in enumerate(header):
      data[h].append(ldata[i])

  # print(data)

  data = pd.DataFrame(data)
  data_raw = data.copy()


  # print(data)

  numeric_headers = ["Slack", "Latency (cycles)", "Latency (ns)", "Iteration Latency", "Interval", "Trip Count"]
  resource_headers = ["BRAM", "DSP", "FF", "LUT", "URAM"]

  for i in range(len(data)):
    for h in header:
      d = data.loc[i, h]
      if h == "Modules & Loops":
        d = d[2:]
        if "ap_fixed" in d:
          tmp = d.split("_")
          d = tmp[0] + "_" + tmp[-2]
        data.loc[i, h] = d
      elif h in numeric_headers:
        if d == '-':
          # d = -1 # idk if better to leave with -1 or 0
          d = 0
        data.loc[i, h] = pd.to_numeric(d)
      elif h == 'Pipelined':
        data.loc[i, h] = True if d == 'yes' else False
      elif h in resource_headers:
        if d == '-':
          raw = 0
          percent = 0
        else:
          raw, percent = d.split(" ")
          percent = float(raw) / MAX_VALS[h] * 100
          # percent = re.findall("\d{1,3}%", percent)[0][:-1]


        data.loc[i, h] = raw
        data.loc[i, h + " %"] = percent

  data["Track Size"] = tracksize
  data["Timing (min)"] = timing

  # resource_headers += [h + " %" for h in resource_headers]
  resource_percent_headers = [h + " %" for h in resource_headers]

  typedict = {}
  for h in resource_headers:
    typedict[h] = int
  for h in resource_percent_headers:
    typedict[h] = float
  for h in numeric_headers:
    typedict[h] = int
  typedict["Pipelined"] = bool
  typedict["Modules & Loops"] = str
  typedict["Issue Type"] = str
  typedict["Slack"] =  float
  data = data.astype(typedict)

  # print(data.dtypes)
  # data

  data.to_csv(out_name + ".csv", index=False)

  return data

if __name__ == "__main__":
  tracksize = -1

  file = open("../src/common/projectDefines.h", 'r').read()
  index = file.find("MAX_TRACK_SIZE")
  indexend = file.find("\n", index)
  if index != -1:
    tracksize = int(file[index+len("MAX_TRACK_SIZE"):indexend])

  print("tracks:",tracksize)

  if len(sys.argv) < 3+1:
    print("python {} <path> <minutes> <seconds>".format(sys.argv[0]))
    exit(0)

  minutes_taken = int(sys.argv[2])
  seconds_taken = int(sys.argv[3])

  timing = minutes_taken + seconds_taken/60
  # prefix = "./July10_4_data/"
  prefix = sys.argv[1]
  out_name = prefix + str(tracksize) + "TS"
  Path(prefix).mkdir(parents=True, exist_ok=True)
  Path(prefix + "info.txt").touch(exist_ok=True)
  data = saveReport(tracksize=tracksize, timing=timing, out_name=out_name)

  print("Finished saving")