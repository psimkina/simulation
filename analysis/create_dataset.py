#from ntuple_loader import df, event
import numpy as np 
import random
import pandas as pd

#df = pd.read_pickle('df')
#event = pd.read_pickle('ev')

n = 10000
print("Number of events: ", n)
def to_numpy(df, event):
    """
    Function to convert information from pandas dataframe to the numpy array. 
    
    Return:
    - X: numpy array (dim: nx25x25) with simulated energy info.
    - simulated: numpy array (dim: nx7x7) with each simulated cluster in the center of the array.
    - center: numpy array (dim: nx2) with simulated position of the primary particle.
    - energy: numpy array (dim: n) with simulated momentum of primary particle. 
    
    Args: 
    - df: dataframe containing information about energy deposit of each event. 
    - event: dataframe containing information about primary event. 
    """
    # Define empty numpy arrays.
    indices = event.index.tolist()
    n = len(indices)
    indices = np.asarray(indices)
    
    
    X = np.full((n, 51, 51), 0.)
    simulated = np.full((n, 7, 7), 0.)
    center = np.full((n, 2), 0.)
    energy = np.full((n), 0.)
    ptype = np.full((n), 0.)
    
    # Fill the numpy arrays with information from dataframe.  
    for s, ind in enumerate(indices):
        if (ind%1000 == 0): print("samples processed:", ind)
        row = df.loc[ind].Row.astype(int)
        col = df.loc[ind].Column.astype(int)
        X[s, row, col] = df.loc[ind].EnergyVector
        center[s, 0], center[s, 1] = event.loc[ind].InitialRow, event.loc[ind].InitialColumn
        energy[s] = event.loc[ind].InitialMomentum
        ptype[s] = event.loc[ind].ParticleType
        
        row_init = round(event.loc[ind].InitialRow)
        col_init = round(event.loc[ind].InitialColumn)
        
        # Change the position of the cluster, so that it is in the center of the numpy array. 
        one_event = df.loc[ind]
        one_event = one_event[(one_event.Row.isin(np.arange(row_init-3,row_init+4)))\
                              &(one_event.Column.isin(np.arange(col_init-3,col_init+4)))]
        
        k = one_event.Row.astype(int) - row_init + 3
        l = one_event.Column.astype(int) - col_init + 3
        simulated[s, k, l] = one_event.EnergyVector

    return X, simulated, center, energy, ptype

#X, sim, center, en, partype = to_numpy(df, event)

def create_sample(X, sim, center, en, partype, k=1):
    """
    Randomly combines clusters from different events into one sample and returns data to be fed 
    into the network:
    
    - X_sim: numpy array (dim: 25x25) containing simulated energy information of the clusters.
    - ysim: numpy array (dim: kx7x7) with sim energy per pixel info of each cluster.
    - ysort: numpy array (dim: kx2) with center sim coordinates of each cluster.
    - ye: numpy array (dim: k) with sim energy info of each cluster.
    
    Args: 
    - X: numpy array with simulated energy info. 
    - sim: numpy array with energy deposition per pixel. 
    - center: numoy array with sim coordinates of the cluster center. 
    - en: numpy array with total energy info for each cluster.
    - k (default = 3): number of clusters per sample. 
    """
    # Randomly choose events and the number of clusters and fill the arrays. 
    random.random()
    t = random.randint(1, k)
    index = random.sample(list(np.arange(0, n)), t)

    X_sim = sum((X[ind] for ind in index))
    yc = np.asarray([(center[ind]) for ind in index])
    ye = np.asarray([(en[ind]) for ind in index])
    yp = np.asarray([(partype[ind]) for ind in index])
    ysim = np.asarray([(sim[ind]) for ind in index])
    
    # Sort the clusters based on the distance from (0,0) coordinate of the image. 
    dr = np.sqrt(1**2 + 1**2)
    ye = ye[np.argsort(-yc[:,0])] 
    yp = yp[np.argsort(-yc[:,0])] 
    ysim = ysim[np.argsort(-yc[:,0])]
    ysort = yc[np.argsort(-yc[:,0])] 

    return X_sim, np.pad(ysim,((0, k-t),(0,0), (0,0)), constant_values=0.),\
        np.pad(ysort, ((0,k-t),(0,0)), constant_values=-1),\
        np.pad(ye, (0,k-t), constant_values=-1),\
        np.pad(yp, (0,k-t), constant_values=-1)
#X, X_sme, yc, ye, ysim = create_sample(X, sme, sim, center, en) 

def save_sample(name, X, sim, center, en, partype, nevt=10, s=0, k=1):
    """
    Create and save data sample in .npy format.
    
    Args: 
    - name: name of the file to save into.
    - nevent (default = 10000): number of samples to save in one file.
    - s (default = 0): random seed.
    - k (default = 1): number of clusters per image. 
    """
    random.seed(s)
    # Create numpy arrays to be filled. 
    Xsim = np.full((nevt, 51, 51), 0.)
    yc = np.full((nevt, k, 2), 0.)
    ysim = np.full((nevt, k, 7, 7), 0.)
    ye = np.full((nevt, k), 0.)
    yp = np.full((nevt, k), 0.)
    
    # Fill the arrays.
    for i in range(nevt):
        if (i%10000 == 0): print('samples made: ', i)
        Xsim[i], ysim[i], yc[i], ye[i], yp[i] = create_sample(X, sim, center, en, partype, k)

    # Smearing of the simultation. 
    Xsim = np.random.normal(Xsim, 0.1) # sigma = 100 MeV.
    Xsim[Xsim<0.05] = 0.0 # cut on smeared energy 50 MeV. 
    
    print('Start smearing')
    
    ysme = np.full((nevt, k, 7, 7), 0.)
    for i, sample in enumerate(Xsim):
        for j in range(k):
            array = sample[max(int(yc[i,j,0])-3,0):min(51,int(yc[i,j,0])+4),max(0,int(yc[i,j,1])-3):min(int(yc[i,j,1])+4,51)]
            ysme[i,j] = np.pad(array, ((0,7-array.shape[0]),(0,7-array.shape[1])))
    
    ysim[ysim<0.05] = 0.0
    ysme[ysme<0.05] = 0.0
    # Normalize the center coordinates to (0,1). 
    yc/=51.
    # Reshape to fit the input for the neural network.
    Xsim = Xsim.reshape(nevt,51,51,1)
        
    # Save files. 
    np.save(str(name) + 'X', Xsim)
    np.save(str(name)+'yc', yc)
    np.save(str(name)+'ysim', ysim)
    np.save(str(name)+'en', ye)
    np.save(str(name)+'ysme', ysme)
   # np.save(str(name)+'yptype', yp)
    

    
