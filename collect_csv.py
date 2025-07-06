import glob
import pandas as pd

directory = "FPGA_MODELS"
#directory = "FPGA_MODELS_NO_PIPELINE"

paths = glob.glob(f"{directory}/builds/*")
paths.sort()

df = pd.DataFrame()

for path in paths:
    try:
        csv = pd.read_csv(f"{path}/out.csv")
        name = csv["name"][0]
        name = name[35:35+3]
        hd = int(name)
        csv["nHidden"] = hd
        csv = csv.drop("Unnamed: 0", axis=1)
        csv = csv.set_index("nHidden")
        print(csv)
        #pd.concat(df, csv)
        df = pd.concat([df, csv])
    except:
        print(f"Cannot find {path}")

print(df)

df.to_csv(f"full_{directory}_stats.csv")

