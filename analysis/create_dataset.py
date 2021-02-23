from ntuple_loader import df, event
import numpy as np 
import random

n = event.index.size
print("Number of events: ", n)
def to_numpy(df, event):
    """
    Function to convert information from pandas dataframe to the numpy array. 
    
    Return:
    - X: numpy array (dim: nx25x25) with simulated energy info.
    - smeared: numpy array (dim: nx25x25) with smeared energy info.
    - simulated: numpy array (dim: nx7x7) with each simulated cluster in the center of the array.
    - center: numpy array (dim: nx2) with simulated position of the primary particle.
    - energy: numpy array (dim: n) with simulated momentum of primary particle. 
    
    Args: 
    - df: dataframe containing information about energy deposit of each event. 
    - event: dataframe containing information about primary event. 
    """
    # Define empty numpy arrays.
    X = np.full((n, 25, 25), 0.)
    smeared = np.full((n, 25, 25), 0.)
    simulated = np.full((n, 7, 7), 0.)
    center = np.full((n, 2), 0.)
    energy = np.full((n), 0.)
    
    # Fill the numpy arrays with information from dataframe.  
    for ind in range(n):
        X[ind, df.loc[ind].Row, df.loc[ind].Column] = df.loc[ind].EnergyVector
        smeared[ind, df.loc[ind].Row, df.loc[ind].Column] = df.loc[ind].EnergySmeared
        center[ind, 0], center[ind, 1] = event.loc[ind].InitialRow, event.loc[ind].InitialColumn
        energy[ind] = event.loc[ind].InitialMomentum
        
        # Change the position of the cluster, so that it is in the center of the numpy array. 
        one_event = df.loc[ind]
        one_event = one_event[(one_event.Row.isin(np.arange(event.loc[ind].InitialRow-3,event.loc[ind].InitialRow+4)))\
                              &(one_event.Column.isin(np.arange(event.loc[ind].InitialColumn-3,\
                                                                event.loc[ind].InitialColumn+4)))]
        k = one_event.Row - int(event.loc[ind].InitialRow) + 3
        l = one_event.Column - int(event.loc[ind].InitialColumn) + 3
        simulated[ind, k, l] = one_event.EnergySmeared
        
    return X, smeared, simulated, center, energy

X, sme, sim, center, en = to_numpy(df, event)

def create_sample(X, sme, sim, center, en, k=3):
    """
    Randomly combines clusters from different events into one sample and returns data to be fed 
    into the network:
    
    - X_sim: numpy array (dim: 25x25) containing simulated energy information of the clusters.
    - X_sme: numpy array (dim: 25x25) containing smeared energy information of the clusters.
    - ysort: numpy array (dim: kx2) with center sim coordinates of each cluster.
    - ye: numpy array (dim: k) with sim energy info of each cluster.
    - ysim: numpy array (dim: kx7x7) with sim energy per pixel info of each cluster.
    
    Args: 
    - X: numpy array with simulated energy info. 
    - sme: numpy array with smeared energy info.
    - sim: numpy array with energy deposition per pixel. 
    - center: numoy array with sim coordinates of the cluster center. 
    - en: numpy array with total energy info for each cluster.
    - k (default = 3): number of clusters per sample. 
    """
    # Randomly choose events and the number of clusters and fill the arrays. 
    random.random()
    index = random.sample(list(np.arange(0,n)), k)

    X_sim = sum((X[ind] for ind in index))
    X_sme = sum((sme[ind] for ind in index))
    yc = np.asarray([(center[ind]) for ind in index])
    ye = np.asarray([(en[ind]) for ind in index])
    ysim = np.asarray([(sim[ind]) for ind in index])
    
    # Sort the clusters based on the distance from (0,0) coordinate of the image. 
    dr = np.sqrt(1**2 + 1**2)
    ysort = yc[np.argsort((np.sqrt(np.power(yc[:,0],2) + np.power(yc[:,1],2))//dr + yc[:,0]/25))] 
    ye = ye[np.argsort((np.sqrt(np.power(yc[:,0],2) + np.power(yc[:,1],2))//dr + yc[:,0]/25))] 
    ysim = ysim[np.argsort((np.sqrt(np.power(yc[:,0],2) + np.power(yc[:,1],2))//dr + yc[:,0]/25))] 

    return X_sim, X_sme, ysort, ye, ysim
X, X_sme, yc, ye, ysim = create_sample(X, sme, sim, center, en) 

def save_sample(name, nevt=1000, s=0):
    """
    Create and save data sample in .npy format.
    
    Args: 
    - name: name of the file to save into.
    - nevent (default = 10000): number of samples to save in one file.
    - s (default = 0): random seed.
    """
    random.seed(s)
    # Create numpy arrays to be filled. 
    X = np.full((nevt, 25, 25), 0.)
    yc = np.full((nevt, 3, 2), 0.)
    ysim = np.full((nevt, 3, 7, 7), 0.)
    #ye = np.full((nevt, 5), 0.)
    
    # Fill the arrays.
    for i in range(nevt):
        simul, X[i], yc[i], ener, ysim[i] = create_sample(X, sme, sim, center, en)
    # Normalize the center coordinates to (0,1). 
    yc/=25
    X = X.reshape(nevt,25,25,1)
        
    # Save files. 
    np.save('X' + str(name), X)
    np.save('yc' + str(name), yc)
    np.save('ysim' + str(name), ysim)
    

    