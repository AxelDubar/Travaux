

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import javafx.scene.canvas.GraphicsContext;

public class EnveloppeConvexe {

	Set<Points> list = new TreeSet<>();

	public EnveloppeConvexe(Set<Points> list) {
		this.list = list;
		build();
	}

	void build() {
		if(list.size()>3) {
			Set<Points> listGauche = new TreeSet<>();
			Set<Points> listDroite = new TreeSet<>();

			for(int index = 0; index < list.size(); index++) {
				if(index<list.size()/2)
					listGauche.add((Points)list.toArray()[index]);
				else {
					listDroite.add((Points)list.toArray()[index]);
				}
			}
			EnveloppeConvexe gauche=new EnveloppeConvexe(listGauche);
			EnveloppeConvexe droite=new EnveloppeConvexe(listDroite);

			gauche.build();
			droite.build();

			reconstruction(gauche, droite);
		}
		else if(list.size() == 3) {
			Points p1 = (Points)list.toArray()[0];
			Points p2 = (Points)list.toArray()[1];
			Points p3 = (Points)list.toArray()[2];

			int xu=(int)(p1.getX()-p2.getX());
			int xv=(int)(p1.getX()-p3.getX());

			int yu=(int)(p1.getY()-p2.getY());
			int yv=(int)(p1.getY()-p3.getY());

			if(xu*yv - xv*yu > 0) {
				p1.setSuiv(p2);
				p2.setSuiv(p3);
				p3.setSuiv(p1);
			}
			else {
				p1.setSuiv(p3);
				p3.setSuiv(p2);
				p2.setSuiv(p1);
			}


		}
		else{
			Points p1 = (Points)list.toArray()[0];
			Points p2 = (Points)list.toArray()[1];

			p1.setSuiv(p2);
			p2.setSuiv(p1);
		}
	}


	private void reconstruction(EnveloppeConvexe gauche, EnveloppeConvexe droite) {
		Points pointGauche=pointDroite(gauche);
		Points pointDroite=pointGauche(droite);

		Points pointGaucheHaut=pointGauche;
		Points pointDroitHaut=pointDroite;
		Points pointGaucheBas=pointGauche;
		Points pointDroitBas=pointDroite;


		double xu,yu,xv,yv;

		int nbEchec=0;
		while (nbEchec<2) {//arrête du haut
			switch(nbEchec) {
			case 0:
				xu=(pointGaucheHaut.getX()-pointDroitHaut.getX());
				yu=(pointGaucheHaut.getY()-pointDroitHaut.getY());

				xv=(pointGaucheHaut.pre.getX()-pointDroitHaut.getX());
				yv=(pointGaucheHaut.pre.getY()-pointDroitHaut.getY());
				if(xu*yv - xv*yu <= 0) {
					nbEchec++;
				}
				else 
					pointGaucheHaut=pointGaucheHaut.pre;
				break;
			case 1:
				xu=(pointDroitHaut.getX()-pointGaucheHaut.getX());
				yu=(pointDroitHaut.getY()-pointGaucheHaut.getY());

				xv=(pointDroitHaut.suiv.getX()-pointGaucheHaut.getX());
				yv=(pointDroitHaut.suiv.getY()-pointGaucheHaut.getY());
				if(xu*yv - xv*yu >= 0) {
					nbEchec++;
				}
				else {
					pointDroitHaut=pointDroitHaut.suiv;
					nbEchec=0;
				}
				break;
			default:
			}
		}
		nbEchec=0;
		while (nbEchec<2) {//arrête du bas

			//System.out.println(nbEchec);
			switch(nbEchec) {
			case 0:
				xu=(int)(pointGaucheBas.getX()-pointDroitBas.getX());
				yu=(int)(pointGaucheBas.getY()-pointDroitBas.getY());

				xv=(int)(pointGaucheBas.suiv.getX()-pointDroitBas.getX());
				yv=(int)(pointGaucheBas.suiv.getY()-pointDroitBas.getY());
				if(xu*yv - xv*yu >= 0) {
					nbEchec++;
				}
				else 
					pointGaucheBas=pointGaucheBas.suiv;
				break;
			case 1:
				xu=(int)(pointDroitBas.getX()-pointGaucheBas.getX());
				yu=(int)(pointDroitBas.getY()-pointGaucheBas.getY());

				xv=(int)(pointDroitBas.pre.getX()-pointGaucheBas.getX());
				yv=(int)(pointDroitBas.pre.getY()-pointGaucheBas.getY());
				if(xu*yv - xv*yu <= 0) {
					nbEchec++;
				}
				else {
					pointDroitBas=pointDroitBas.pre;
					nbEchec=0;
				}
				break;
			default:
			}
		}

		pointGaucheHaut.setSuiv(pointDroitHaut);
		pointDroitBas.setSuiv(pointGaucheBas);

	}

	private Points pointDroite(EnveloppeConvexe gauche) {
		return (Points) gauche.list.toArray()[gauche.list.size()-1];
	}

	private Points pointGauche(EnveloppeConvexe droite) {
		return (Points) droite.list.toArray()[0];
	}

	public void draw(GraphicsContext graphicsContext) {
		Points pointDepart=pointGauche(this);
		Points point=pointDepart;
		do{
			graphicsContext.strokeLine(point.getX(), point.getY(), point.suiv.getX(), point.suiv.getY());
			point=point.suiv;
		}while(point!=pointDepart);
	}
	
	List<Points> enveloppe(){
		List<Points> points = new ArrayList<>();
		points.add(pointGauche(this));
		Points point=pointGauche(this).suiv;
		do{
			points.add(point);
			point=point.suiv;
		}while(point!=points.get(0));
		return points;
		
	}
	
	List<Points> pasEnveloppe(){
		List<Points> pasEnveloppe = new ArrayList<>();
		pasEnveloppe.addAll(list);
		pasEnveloppe.removeAll(enveloppe());
		return pasEnveloppe;
	}

}
