// Homelab Sensor Case — Parametric OpenSCAD
// For: XIAO ESP32-C6 + GY-SHT31-D breakout
// Print: PLA/PETG, 0.2mm layers, 20% infill, no supports

/* [Dimensions] */
wall = 1.6;           // Wall thickness
inner_w = 21;         // Inner width (XIAO is ~17.5mm)
inner_l = 35;         // Inner length
inner_h = 12;         // Inner height

/* [USB Cutout] */
usb_w = 9.5;          // USB-C width
usb_h = 3.5;          // USB-C height
usb_z = 1.5;          // USB-C offset from floor

/* [Vent Grid] */
vent_slots = 5;       // Number of vent slots
vent_w = 2;           // Slot width
vent_gap = 1.5;       // Gap between slots
vent_offset_x = 3;    // Offset from SHT31 end

/* [Mount Holes] */
mount_d = 4;          // Mount hole diameter
mount_inset = 4;      // Inset from edges

/* [Snap Clips] */
clip_w = 3;           // Clip width
clip_d = 0.8;         // Clip depth (snap distance)
clip_h = 3;           // Clip height

/* [PCB Standoffs] */
standoff_h = 2;       // Height above floor
standoff_d = 3;       // Standoff diameter
standoff_hole = 1.2;  // Screw hole (M1.2)

// Derived
outer_w = inner_w + 2*wall;
outer_l = inner_l + 2*wall;
outer_h = inner_h + wall;  // Floor only, lid is separate
lid_h = 2;
$fn = 30;

// --- BODY ---
module body() {
    difference() {
        // Outer shell
        cube([outer_w, outer_l, outer_h]);
        // Inner cavity
        translate([wall, wall, wall])
            cube([inner_w, inner_l, inner_h + 1]);  // +1 to open top
        // USB-C cutout (short end, Y=0)
        translate([(outer_w - usb_w)/2, -0.1, wall + usb_z])
            cube([usb_w, wall + 0.2, usb_h]);
        // Mount holes (back wall, through)
        translate([mount_inset, outer_l/2, -0.1])
            cylinder(d=mount_d, h=wall + 0.2);
        translate([outer_w - mount_inset, outer_l/2, -0.1])
            cylinder(d=mount_d, h=wall + 0.2);
    }
    // PCB standoffs (XIAO positions, approximate)
    for (x = [wall + 2, wall + inner_w - 2])
        for (y = [wall + 3, wall + 20])
            translate([x, y, wall])
                difference() {
                    cylinder(d=standoff_d, h=standoff_h);
                    cylinder(d=standoff_hole, h=standoff_h + 0.1);
                }
    // SHT31 shelf (elevated platform at far end)
    translate([wall + 2, wall + inner_l - 12, wall])
        cube([inner_w - 4, 10, standoff_h]);
    // Snap clip receivers (inside walls)
    for (x = [wall - 0.1, outer_w - wall - clip_w + 0.1])
        for (y = [wall + 8, wall + inner_l - 8 - clip_w])
            translate([x, y, outer_h - clip_h])
                cube([clip_w, clip_w, clip_h]);
}

// --- LID ---
module lid() {
    difference() {
        union() {
            // Lid plate
            cube([outer_w, outer_l, lid_h]);
            // Inner lip (fits inside body)
            translate([wall + 0.2, wall + 0.2, -1.5])
                cube([inner_w - 0.4, inner_l - 0.4, 1.5]);
        }
        // Vent grid (over SHT31 area)
        vent_total = vent_slots * vent_w + (vent_slots - 1) * vent_gap;
        vent_start_y = outer_l - vent_offset_x - wall - vent_total;
        for (i = [0:vent_slots-1])
            translate([wall + 3, vent_start_y + i * (vent_w + vent_gap), -0.1])
                cube([inner_w - 6, vent_w, lid_h + 0.2]);
    }
    // Snap clips (on lid underside)
    for (x = [wall + 0.3, outer_w - wall - clip_w + 0.3])
        for (y = [wall + 8, wall + inner_l - 8 - clip_w])
            translate([x, y, -1.5 - clip_h]) {
                cube([clip_w - 0.6, clip_w, clip_h]);
                // Snap bump
                translate([0, 0, 0])
                    cube([clip_w - 0.6, clip_w, clip_d]);
            }
}

// --- RENDER ---
// Body
body();
// Lid (offset for printing side by side)
translate([outer_w + 5, 0, lid_h])
    rotate([180, 0, 0])
        lid();
