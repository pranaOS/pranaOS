/**
 * @file try.java
 * @author Krisna Pranav
 * @brief TRY
 * @version 6.0
 * @date 2023-06-27
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

package kernel;

import java.util.*;
import java.sql.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Try extends JFrame {
    static JFrame f;
    static JLabel l;

    public static void main(String[] args) {
        boolean resizable = true;
        boolean closable = true;
        boolean maximizable = true;
        boolean iconifiable = true;

        f = new JFrame("frame");

        JInternalFrame in = new JInternalFrame("Frame title3", resizable);

        in.setBounds(100,100,100,100);
        in.setTitle("Internal Frame");

        JButton b = new JButton("Button");

        l = new JLabel("This is JInternal Frame");

        JPanel p = new JPanel();

        p.add(l);
        p.add(b);

        in.setVisible(true);
        in.add(p);

        f.add(in);
        f.setSize(1300, 300);
        f.setVisible(true);
    }
}
