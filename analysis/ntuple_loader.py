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
df['EnergyVector']/=1000
df = df[['Row','Column', 'EnergyVector']][df['EnergyVector']>0.05] # Condition on what values to leave in the df. 

# Define the smeared energy at each cell.
def get_parameters(E, a=0.02, b=0., c=0.):
    """
    Function to calculate the parameters (based on energy resolution) 
    to be fed into the gaussian distribution. 
    
    Return: 
    - mu: mean of the distribution (energy in this case). 
    - sigma: standard deviation. 
    
    Args: 
    - E: true energy (in MeV) deposited in the cell.
    - a (default=0.03): stochastic term of energy resolution. 
    - b (default=250 MeV): noise term of energy resolution. 
    - c (default=0.005): constant term of energy resolution. 
    """
    mu = E
    sigma = E*np.sqrt((a/np.sqrt(E))**2 + (b/E)**2 + c**2)
    return mu, sigma 

# Calculate the smeared energy and add it to the df. 
mu, sigma = get_parameters(df['EnergyVector'])
smear = np.random.normal(mu, sigma) 
df['EnergySmeared'] = smear
df['EnergySmeared'][df['EnergySmeared']<0.05] = 0