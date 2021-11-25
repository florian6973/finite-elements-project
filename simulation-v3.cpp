load "medit"
include "cube.idp"

verbosity = 0;

// question : comment retourner un tuple simuler déplacement pour faire évolution maillage ? OK normalement
// question : définition de la normale n ? surface 2d ok ?
// quesiton : càd moitié de la structure ?
// question : pont pas variable globale ? NON
// question : comment définir la région  sur laquelle intégrer ?
// rédaction ?
// todo commenter code
// save img & half structure
// medit to show labels

real sqrt2=sqrt(2.);
macro epsilon(u1,u2,u3)  [dx(u1),dy(u2),dz(u3),(dz(u2)+dy(u3))/sqrt2,(dz(u1)+dx(u3))/sqrt2,(dy(u1)+dx(u2))/sqrt2] // EOM
macro div(u1,u2,u3) ( dx(u1)+dy(u2)+dz(u3) ) // EOM

// Géométrie
real r = 0.35;
real h = 0.4;
real l = 1.7-r*2;
real p = 0.5;

// Grandeurs mécaniques
real fz = -5e8;
real E = 2.1e11;
real nu = 0.3;
real g = -9.81;
real rho = 7800;

// Grandeurs calculées
real gravity = rho*g;
real mu = E/(2*(1+nu));
real lambda = E*nu/((1+nu)*(1-2*nu));

mesh3 pont = cube(2,2,2); // default
fespace Vh(pont,[P1,P1,P1]); // P1 ou P2 ?
Vh [u1,u2,u3], [v1,v2,v3];

func mesh build2d(bool show, bool half, real e, int n)
{
    border b1(t=1, -1){x=t*l/2; y=0; label=1;};
    border b2(t=-1, 1){x=t*l/2; y=-e; label=2;};

    border b3(t=0, pi/2){x=r*cos(t)+l/2; y=r*sin(t)-r; label=3;};
    border b4(t=pi/2, 0){x=(r-e)*cos(t)+l/2; y=(r-e)*sin(t)-r; label=4;};

    border b5(t=0, 1){x=l/2+r-e; y=-t*h-r; label=5;};
    border b6(t=1, 0){x=l/2+r; y=-t*h-r; label=6;};
    border b7(t=1, 0){x=l/2+r-e*t; y=-h-r; label=7;};


    border b8(t=pi/2, 0){x=-(r*cos(t)+l/2); y=r*sin(t)-r; label=8;};
    border b9(t=0, pi/2){x=-((r-e)*cos(t)+l/2); y=(r-e)*sin(t)-r; label=9;};

    border b10(t=1, 0){x=-(l/2+r-e); y=-t*h-r; label=10;};
    border b11(t=0, 1){x=-(l/2+r); y=-t*h-r; label=11;};
    border b12(t=0, 1){x=-(l/2+r-e*t); y=-h-r; label=12;};

    border b13(t=1,0){x=0; y=t*e-e;label=18;};


    border b14(t=1, 0){x=t*l/2; y=0; label=1;};
    border b15(t=0, 1){x=t*l/2; y=-e; label=2;};
    border b16(t=1, 0){x=0; y=t*e-e; label=8;};

    mesh pont2d = buildmesh(b1(n) + b2(n) + b3(n) + b4(n) + b5(n) + b6(n) + b7(n) + b8(n) + b9(n) + b10(n) + b11(n) + b12(n));
    if (half)
    {
        pont2d = buildmesh(b3(n) + b4(n) + b5(n) + b6(n) + b7(n) + b14(n) + b15(n) + b16(n));
    }
    if (show)
    {        
        plot(pont2d, wait=true);
    }

    return pont2d;
}

func mesh3 build3d(mesh pont2d, bool show, int m)
{
    mesh3 pontretourne = buildlayers(pont2d, m, zbound=[-p/2,p/2]);  
    pont = movemesh3(pontretourne, transfo=[x, z, y], orientation=-1);

    if (show)
    {
        plot(pont, wait=true);
    }

    return pont;
}

func real simulerdeplacement(bool show, real e, int n, int m, bool adapt)
{
    [u1, u2, u3] = [0, 0, 0]; // pour mettre à jour le maillage
    
    solve Lame([u1,u2,u3],[v1,v2,v3])=
    int3d(pont)(  
            lambda*div(u1,u2,u3)*div(v1,v2,v3)	
            +2.*mu*( epsilon(u1,u2,u3)'*epsilon(v1,v2,v3) )
            )
    - int3d(pont) (gravity*v3)
    + on(7,u1=0,u2=0,u3=0) // condition bord bas (Dirichlet)
    + on(12,u1=0,u2=0,u3=0) // condition bord bas (Dirichlet)
    - int2d(pont, 1)(fz*v3) // condition force surfacique (Neumann)
    ;

    real dmax= u3[].min; // min ou max ?

    if (show)
    {        
        real coef= 0.1/abs(dmax);
        int[int] ref2=[1,0,2,0];
        mesh3 pontdeforme=movemesh3(pont,transfo=[x+u1*coef,y+u2*coef,z+u3*coef],label=ref2);
        //pont=change(pontdeforme,label=ref2);
        string geom = " | (n,m)=" + n + "," + m;
        if (adapt)
        {
            geom = " | auto";
        }
        plot(pontdeforme, wait=1,cmm="e="+e+" | coef  amplification="+coef+" | dep=" + dmax + geom, ps="plot.ps");
        //exporter en vtk puis paraview
        //enregistrer les solutions, u1, U2, u3 et maillage : write to file (ofstream, save mesh)
    }

    return dmax;
}

func real deplacementmax(bool show, bool half, real e, int n, int m)
{
    mesh pont2d = build2d(show, half, e, n);
    pont = build3d(pont2d, show, m);
    real dmax = simulerdeplacement(show, e, n, m, false);
    return dmax;
}

string s;
cout << "Tapez s pour voir la configuration classique g pour tracer le graphe m pour simuler avec la moitie de la maille a pour simuler avec ladaptation du mesh d pour la dichotomie b pour adaptation manuelle" << endl;
cin >> s;

int n = 10;
int m = n/4;

if (s == "g")
{
    real e = 0.1;//0.001; pas trop petit pour intéressant
    real inc = 0.006;
    real rmax = 0.3; //r;
    int nb = (rmax-e)/inc + 1;
    real[int] xx(nb), yy(nb);
    int i = 0;

    while (e < rmax)
    {
        real dmax = deplacementmax(false, false, e, n, m);
        xx[i] = e;
        yy[i] = dmax;
        cout << "e=" << e << endl;
        e += inc;
        i += 1;
    }

    {// file for gnuplot
        ofstream gnu("plot.txt");
        for (int i = 0; i < nb; i++)
           gnu << xx[i] << " " << yy[i] << endl;
    }
    plot([xx, yy], wait=true);
}
else if (s == "d")
{

//méthode dichotomie
real enmin, enmax, en, dn;
enmin = 0.1; // m
enmax = 0.3; //m
dn = 77;
en = 0;
while (abs(dn)!=0.1 & abs(abs(dn)-0.1)>=0.001)
{
    en = (enmin+enmax)/2;
    dn = deplacementmax(false, false, en, 10, 10);
    if (abs(dn)<= 0.1)
    {
        enmax = en;
    }
    else
    { 
        enmin = en;
    }
}
dn = deplacementmax(false, false, enmax, 10, 10);

cout << "Le deplacement est de " << dn << "m et lepaisseur " << enmax << "m" <<endl;

}
else if (s == "a") // todo adaptation manuelle
{
    real e = 0.1;   
    bool show = true;
    bool half = false;

    cout << "CTRL-C to stop" << endl;

    mesh pont2d = build2d(show, half, e, n);
    mesh3 pont = build3d(pont2d, show, m);

    while (true)
    {
        simulerdeplacement(show, e, n, m, true);

        pont2d = adaptmesh(pont2d, [u1, u2]);
        if (show)
        {
            plot(pont2d, wait=true);
        }
        pont = build3d(pont2d, show, m);
    }
}
else if (s == "m")
{
    real e = 0.1;   
    bool show = true;
    deplacementmax(show, true, e, n, m);
}
else if (s == "s")
{
    real e = 0.155;   
    bool show = true;
    deplacementmax(show, false, e, n, m);
}
else if (s == "b")
{
    real e = 0.1;   
    bool show = true;
    for (int n2 = n; n2 < 40; n2+=5)
    {
        for (int m2 = m; m2 < 15; m2+=2)
        {
            deplacementmax(show, false, e, n2, m2);
        }
    }
}

 

// grep "mmg3d(" */*.edp
//adapter les paramètres du mesh à la main sinon car contrôle la création