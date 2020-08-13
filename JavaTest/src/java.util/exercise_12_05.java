import java.util.Scanner;

class GeometricObject {
    private String color = "white";
    private boolean filled;
    private java.util.Date dateCreated;

    public GeometricObject() {
        dateCreated = new java.util.Date();
    }

    public GeometricObject(String color, boolean filled) {
        dateCreated = new java.util.Date();
        this.color = color;
        this.filled = filled;
    }

    public String toString() {
        return "created on " + dateCreated + "\ncolor: " + color + " and filled: " + filled;
    }
}

class IllegalTriangleException extends Exception {
    /**
     *
     */
    private static final long serialVersionUID = 1L;

    //private double side1, side2, side3;
    public IllegalTriangleException(double side1, double side2, double side3) {
        super("Invalid side" + " " + side1 + " " + side2 + " " + side3);
        // this.side1=side1;
        // this.side2=side2;
        // this.side3=side3;
    }
}

class Triangle extends GeometricObject {
    private double side1, side2, side3;

    public Triangle() throws IllegalTriangleException {
        this("white", false, 1.0, 1.0, 1.0);
    }

    public Triangle(String color, boolean filled, double side1, double side2, double side3) throws IllegalTriangleException {
        super(color, filled);
        if (side1 + side2 > side3 && side1 + side3 > side2 && side2 + side3 > side1) {
            this.side1 = side1;
            this.side2 = side2;
            this.side3 = side3;
        } else
            throw new IllegalTriangleException(side1, side2, side3);
    }

    public double getArea() {
        double p = this.getPerimeter() / 2.0;
        return Math.sqrt(p * (p - side1) * (p - side2) * (p - side3));
    }

    public double getPerimeter() {
        return side1 + side2 + side3;
    }

    public String toString() {
        return getClass().getName() + ": side1=" + side1 + " side2=" + side2 + " side3=" + side3;
    }
}

public class exercise_12_05 {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        System.out.println("Please enter three sides respectively:");
        double side1 = input.nextDouble();
        double side2 = input.nextDouble();
        double side3 = input.nextDouble();
        System.out.println("Please enter the color of the triangle and if it is filled:");
        String color = input.next();
        boolean filled = input.nextBoolean();
        try {
            GeometricObject g = new GeometricObject(color, filled);
            Triangle triangle = new Triangle(color, filled, side1, side2, side3);

            System.out.println(g.toString());
            System.out.println("" + triangle);
            System.out.println("The area is " + triangle.getArea());
            System.out.println("The perimeter is " + triangle.getPerimeter());
            input.close();
        } catch (IllegalTriangleException ex) {
            System.out.println(ex.getMessage());
        }
    }
}