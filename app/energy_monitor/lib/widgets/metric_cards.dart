import 'package:flutter/material.dart';
import '../constants/metric_descriptions.dart';

class MetricCard extends StatelessWidget {
  final String title;
  final dynamic value;
  final String unit;
  final IconData icon;
  final VoidCallback? onTap;

  // Add const constructor
  const MetricCard({
    super.key,
    required this.title,
    required this.value,
    required this.unit,
    required this.icon,
    this.onTap,
  });

  // Cache formatted value using getter
  String get formattedValue => value?.toString() ?? '0';

  void _showMetricInfo(BuildContext context) {
    showModalBottomSheet(
      context: context,
      backgroundColor: const Color(0xFF1A1A1A),
      builder: (context) => MetricInfoSheet(metric: title),
    );
  }

  @override
  Widget build(BuildContext context) {
    return RepaintBoundary(
      child: InkWell(
        onTap: () => _showMetricInfo(context),
        child: Card(
          color: const Color(0xFF1A1A1A),
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(12.0),
          ),
          child: ListTile(
            leading: Icon(icon, color: const Color(0xFF5DB996), size: 28),
            title: Text(
              title,
              style: const TextStyle(
                color: Colors.white,
                fontWeight: FontWeight.bold,
              ),
            ),
            subtitle: Text(
              '$formattedValue $unit',
              style: const TextStyle(color: Color(0xFFE3F0AF), fontSize: 16),
            ),
          ),
        ),
      ),
    );
  }
}

class MetricInfoSheet extends StatelessWidget {
  final String metric;

  const MetricInfoSheet({super.key, required this.metric});

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(16),
      child: Column(
        mainAxisSize: MainAxisSize.min,
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Text(
            metric,
            style: const TextStyle(
              color: Color(0xFF5DB996),
              fontSize: 20,
              fontWeight: FontWeight.bold,
            ),
          ),
          const SizedBox(height: 8),
          Text(
            MetricDescriptions.descriptions[metric] ?? '',
            style: const TextStyle(
              color: Colors.white70,
              fontSize: 16,
              height: 1.5,
            ),
          ),
          const SizedBox(height: 16),
        ],
      ),
    );
  }
}
