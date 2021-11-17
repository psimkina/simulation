from ntuple_loader import create_df
from create_dataset import to_numpy

import numpy as np

for i in range(37,100):
    df, event = create_df("/eos/user/p/psimkina/clustering_data/ntuple_{}.root".format(i))
    print('File number:', i)
    X, sim, cent, en, _ = to_numpy(df, event)
    
    np.save('/eos/user/p/psimkina/clustering_data/numpy/X-'+str(i), X)
    np.save('/eos/user/p/psimkina/clustering_data/numpy/yc-'+str(i), cent)
    np.save('/eos/user/p/psimkina/clustering_data/numpy/ysim-'+str(i), sim)
    np.save('/eos/user/p/psimkina/clustering_data/numpy/en-'+str(i), en)
