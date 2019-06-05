

import javafx.geometry.Point2D;

public class Points extends Point2D implements Comparable<Points>{
	
	Points pre, suiv;

	public Points(double x, double y) {
		super(x, y);
	}

	public void setPre(Points pre) {
		this.pre = pre;
	}

	public void setSuiv(Points suiv) {
		this.suiv = suiv;
		suiv.setPre(this);
	}

	public boolean hasSuiv() {
		return suiv!=null;
	}
	
	@Override
	public int compareTo(Points o) {
		if((int) (this.getX() - o.getX())==0)
			return (int) (this.getY() - o.getY());
		return (int) (this.getX() - o.getX());
	}
	
	
	

}
