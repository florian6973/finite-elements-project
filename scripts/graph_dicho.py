

import matplotlib.pyplot as plt
 

def main():
    x = []
    y = []
    with open('scripts/plot_cvg_D.txt', 'r') as f:
        lines = f.readlines()
        for l in lines:
            xt, yt = l.split(' ')
            x.append(round(float(xt)*100, 2))
            y.append(round(float(yt)*100, 2))
    # plotting the points
    plt.plot(x, y, linestyle='-', marker='+', color='b')
    plt.plot(x[-1], y[-1], marker='+', color='r', markersize=10)
    #plt.xscale("log")
    
    # naming the x axis
    plt.xlabel('Epaisseur e (cm)')
    # naming the y axis
    plt.ylabel('Déplacement vertical max d (cm)')
    
    # giving a title to my graph
    plt.title("Convergence de l'algorithme de dichotomie")
    
    # function to show the plot
    plt.savefig('imgs/dicho.png')
    plt.show()

if __name__ == "__main__":
    main()