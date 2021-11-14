import uproot
import numpy as np
import pandas as pd

def create_df(filename="splitted_0-24999_events.root"):
    # Load ntuple from the files and convert it to pandas dataframe.
    #upFilePi = uproot.open('~/calo_sim/simulation/build/pions.root')
    with uproot.open("~/calo_sim/simulation/build/data/"+str(filename)) as file:
        #treePi = upFilePi['CaloH']
        treePh = file['CaloH']
        #dfPi = treePi.pandas.df()
        df = treePh.pandas.df()

    df['ParticleType'] = 0
    #dfPi['ParticleType'] = 1
    #print('done')
    #df = df.loc[50000:59999]
    print('done - 2')
    #dfPi = dfPi.loc[25000:]

    #df = pd.concat([dfPh, dfPi])

    # First dataframe with information about primary particle. 
    event = df[['TotalEn', 'InitialMomentum','InitialRow', 'InitialColumn', 'ParticleType']].drop_duplicates()
    event = event.reset_index(level=1, drop=True)
    print('done - 3')
    # Second dataframe with energy deposits of each event. 
    df = df[['Row','Column', 'EnergyVector']][df['EnergyVector']>0.0] # Condition on what values to leave in the df. 
    print('done - 4')
    #df['Row'] = df['Row'].astype(int)
    #df['Column'] = df['Column'].astype(int)
    df['EnergyVector']/=1000.
    return df, event
