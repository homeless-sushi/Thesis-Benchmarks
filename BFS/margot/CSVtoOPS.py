import sys
import json

import pandas as pd

def main() :

    if len(sys.argv) < 2 :
        print(f"Usage: python {sys.argv[0]} inputURL outputURL")
        raise SystemExit
    
    bfs = []
    df : pd.DataFrame = pd.read_csv(sys.argv[1])
    print(df)
    for index, row in df.iterrows() :
        features = {}
        features["DEVICE_TYPE"] = row["DEVICE_TYPE"]
        features["CPU_THREADS_EXP"] = row["CPU_THREADS_EXP"]

        knobs = {}
        knobs["GPU_BLOCK_EXP"] = row["GPU_BLOCK_EXP"]
        knobs["GPU_CHUNK_EXP"] = row["GPU_CHUNK_EXP"]
        knobs["GPU_EDGES_MEMORY"] = row["GPU_EDGES_MEMORY"]
        knobs["GPU_OFFSETS_MEMORY"] = row["GPU_OFFSETS_MEMORY"]

        metrics = {}
        metrics["timing"] = [row["timing_avg"], row["timing_std"]]

        operating_point = {}
        operating_point["features"] = features
        operating_point["knobs"] = knobs
        operating_point["metrics"] = metrics

        bfs.append(operating_point)

    ops = {}
    ops["bfs"] = bfs

    with open(sys.argv[2], "w+") as ops_file: 
        json.dump(ops, ops_file, indent=8)

    
if __name__ == "__main__" :
    main()