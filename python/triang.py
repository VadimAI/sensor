import numpy as np
import matplotlib.pyplot as plt

if __name__ == "__main__":
     
     x = np.linspace(0,100,100)  #imager length, [mm]
     f = 3.6    #focal length, [mm]
     s = 68.0    #length between laser and imager, [mm]
     pxl_size = 0.00375 #pixel size [mm/pixel]     
     
     q = (f * s)/(pxl_size * x)
     
     plt.figure()
     plt.plot(x,q,linewidth=2.0)
     plt.xlabel('x')
     plt.ylabel('q = fs/x')
     plt.grid(True)
     plt.show()
     
     f = open("dist.txt", "r")
     dist = []
     dist = [line.split(',') for line in f]
     f.close()
     
     dist = np.array(dist)
     
     plt.figure()
     plt.plot(dist[0,:-1])
     plt.grid(True)
     plt.title("triangulated distance")
     plt.xlabel("image column")
     plt.ylabel("distance, [mm]")
     plt.show()

     '''
     f = open("angle.txt", "r")
     angle = []
     angle = [line.split(',') for line in f]     
     f.close()
         
     angle = np.array(angle)

     plt.figure()
     plt.plot(angle[0,:-1])
     plt.grid(True)
     plt.title("top angle to object")
     plt.xlabel("image column")
     plt.ylabel("angle, [degrees]")
     plt.show()
     '''
     
     
     

    