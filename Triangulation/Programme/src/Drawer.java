import javafx.scene.canvas.Canvas;

public class Drawer extends Canvas{

	DrawerContext context = new DrawerContext(this);

    public Drawer(int width, int height) {
        super(width,height);
        setFocusTraversable(true);
        setOnMousePressed(event->context.mousePressed(event));
        setOnKeyPressed(event -> context.keyPressed(event));
    }

    public void repaint(){
            this.getGraphicsContext2D().clearRect(0, 0, this.getWidth(), this.getHeight());
    }


}
