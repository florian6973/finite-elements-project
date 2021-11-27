

import matplotlib.pyplot as plt
 

def main():
    x = []
    y = []
    with open('scripts/plot_ed.txt', 'r') as f:
        lines = f.readlines()
        for l in lines:
            xt, yt = l.split(' ')
            x.append(round(float(xt)*100, 2))
            y.append(round(float(yt)*100, 2))

    plt.plot(x, y, linestyle='-', marker='+', color='b')
    
    plt.xlabel('Epaisseur e (cm)')
    plt.ylabel('Déplacement vertical max d (cm)')
    
    plt.title("Evolution de l'affaissement en fonction de l'épaisseur du pont")
    
    plt.savefig('imgs/ed.png')
    plt.show()

if __name__ == "__main__":
    main()