import glob
import subprocess
import time
import datetime
import xmltodict
import pandas as pd
import numpy as np

filepaths = glob.glob(f"FPGA_MODELS/builds/*")
filepaths.sort()

# filepaths = ["FPGA_MODELS/builds/build_network_hd14_nlyr3_nitr3"]

max_memory = 30 # GB
max_memory = max_memory * 1024 * 1024

ps = []
models = []

for i, file in enumerate(filepaths):
  model_info = []
  print("Starting: {}/{} for file {}/out.log".format(i, len(filepaths), file))
  # process = subprocess.Popen("make > out.log 2>&1", shell=True, cwd=file)
  # process = subprocess.Popen("systemd-run --scope -p MemoryLimit=500M ./run.sh", shell=True, cwd=file)
  hd = int(file[35:35+3])
  print(hd)
  process = subprocess.Popen(f"ulimit -Sv {max_memory} && make > out.log 2>&1", shell=True, cwd=file)
  t = time.time()
  print(datetime.datetime.now())
  time_to_complete = (1.31 + 2.16*hd+0.0116*hd*hd)
  print("Est: ", time_to_complete, "min")
  print("Eta: ", datetime.datetime.now() + datetime.timedelta(minutes=time_to_complete))
  process.wait()
  time_taken = time.time() - t
  print("Took: {} min".format(time_taken/60))
  print(datetime.datetime.now())
  with open(file+"/kernels/rungraphnetwork/hls/syn/report/csynth.xml") as ff:
    f = ff.read()
    f = xmltodict.parse(f)
  est = f["profile"]["AreaEstimates"]

  perf = f["profile"]["PerformanceEstimates"]["SummaryOfOverallLatency"]
  res = est["Resources"]
  avl = est["AvailableResources"]

  dict = {
    "nhidden": hd,
    "name": file,
    "compile_time": time_taken/60.,
    "bram": res["BRAM_18K"],
    "dsp": res["DSP"],
    "ff": res["FF"],
    "lut": res["LUT"],
    "uram": res["URAM"],
    "bLatency": perf["Best-caseLatency"],
    "wLatency": perf["Worst-caseLatency"],
    "bTime": perf["Best-caseRealTimeLatency"][:-3],
    "wTime": perf["Worst-caseRealTimeLatency"][:-3],
    "bII": perf["Interval-min"],
    "wII": perf["Interval-max"],
  }

  pd.DataFrame([dict]).to_csv(file+"/out.csv")

  models.append(dict)



  # ps.append(process)

# print("Running", len(ps))
# while len(ps) > 0:
#   print("Waiting left {}".format(len(ps)))
#   ps[0].wait()
#   del ps[0]
#   print("Finished.")

print("\nAll Finished")



