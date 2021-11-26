

import matplotlib.pyplot as plt
 

def main():
    x = []
    y = []
    with open('plot.txt', 'r') as f:
        lines = f.readlines()
        for l in lines:
            xt, yt = l.split(' ')
            x.append(round(float(xt)*100, 2))
            y.append(round(float(yt)*100, 2))
    # plotting the points
    plt.plot(x, y, linestyle='-', marker='+', color='b')
    
    # naming the x axis
    plt.xlabel('Epaisseur e (cm)')
    # naming the y axis
    plt.ylabel('Déplacement vertical max d (cm)')
    
    # giving a title to my graph
    plt.title("Evolution de l'affaissement en fonction de l'épaisseur du pont")
    
    # function to show the plot
    plt.savefig('graph.png')
    plt.show()

if __name__ == "__main__":
    main()