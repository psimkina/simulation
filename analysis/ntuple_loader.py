import uproot
import numpy as np

# Load ntuple from the files and convert it to pandas dataframe.
upFile = uproot.open('~/calo_sim/simulation/build/dataCaloHits.root')
tree = upFile['CaloH']
df = tree.pandas.df()

# First dataframe with information about primary particle. 
event = df[['TotalEn', 'InitialMomentum','InitialRow', 'InitialColumn']].drop_duplicates()
event = event.reset_index(level=1, drop=True)

# Second dataframe with energy deposits of each event. 
df['Row'] = df['Row'].astype(int)
df['Column'] = df['Column'].astype(int)
df['EnergyVector']/=1000.
df = df[['Row','Column', 'EnergyVector']][df['EnergyVector']>0.0] # Condition on what values to leave in the df. 