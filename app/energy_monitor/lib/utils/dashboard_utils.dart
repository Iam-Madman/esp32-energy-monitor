import 'package:flutter/material.dart';
import 'package:intl/intl.dart';

class DashboardUtils {
  static IconData getBatteryIcon(int percentage, {bool isCharging = false}) {
    if (percentage >= 90) return Icons.battery_full;
    if (percentage >= 80) return Icons.battery_6_bar;
    if (percentage >= 60) return Icons.battery_5_bar;
    if (percentage >= 50) return Icons.battery_4_bar;
    if (percentage >= 40) return Icons.battery_3_bar;
    if (percentage >= 30) return Icons.battery_2_bar;
    if (percentage >= 20) return Icons.battery_1_bar;
    return Icons.battery_alert;
  }

  static void showBatteryWarning(BuildContext context, int battery) {
    if (battery <= 5) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
          content: Text('Critical Battery Level! Please charge immediately.'),
          backgroundColor: Colors.red,
          duration: Duration(seconds: 5),
        ),
      );
    } else if (battery <= 10) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
          content: Text('Low Battery Warning! Connect charger soon.'),
          backgroundColor: Colors.orange,
          duration: Duration(seconds: 4),
        ),
      );
    }
  }

  static String formatTimestamp(int timestamp) {
    final date = DateTime.fromMillisecondsSinceEpoch(timestamp * 1000);
    return DateFormat('dd MMM, hh:mm a').format(date);
  }
}
