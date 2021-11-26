

import matplotlib.pyplot as plt
 
def read_data(file, half = False):    
    x = []
    y = []
    name = 'scripts/' + file
    if half:
        name += "H"
    with open(name + ".txt", 'r') as f:
        lines = f.readlines()
        for l in lines:
            xt, yt = l.split(' ')
            x.append(int(xt))
            y.append(round(float(yt)*100, 2))

    return x, y

colors=['b', 'r', 'y']
ind_col=0

def plot_sim(ax1, x_man, y_man, label):
    global ind_col
    ax1.plot(x_man, y_man, linestyle='-', marker='+', color=colors[ind_col%len(colors)], label=label)
    ax1.set_xlabel('Itération')
    ax1.set_ylabel('Déplacement vertical max d (cm)')
    ax1.set_ylim(-50,-32.5)
    ax1.legend()
    ind_col += 1
    
def plot_type(half):        
    x_man, y_man = read_data('plot_cvg_man', half)
    x_sman, y_sman = read_data('plot_cvg_sman', half)
    x_auto, y_auto = read_data('plot_cvg_auto', half)

    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(12, 4))

    plot_sim(ax1, x_man, y_man, "Adaptation manuelle")
    plot_sim(ax2, x_sman, y_sman, "Adaptation semi-automatique")
    plot_sim(ax3, x_auto, y_auto, "Adaptation automatique")
   
    if not half:
        fig.suptitle('Convergence des résultats de simulation (déplacement vertical) - structure entière')
    else:
        fig.suptitle('Convergence des résultats de simulation (déplacement vertical) - demi structure')
    fig.tight_layout()

    if not half:
        plt.savefig('imgs/cvg.png')
    else:        
        plt.savefig('imgs/cvgH.png')
    plt.legend()
    plt.show()

def main():
    plot_type(False)
    plot_type(True)

if __name__ == "__main__":
    main()