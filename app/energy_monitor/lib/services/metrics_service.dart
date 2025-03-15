import 'package:flutter/foundation.dart';

class MetricsService {
  static final MetricsService _instance = MetricsService._internal();
  factory MetricsService() => _instance;
  MetricsService._internal();

  final ValueNotifier<Map<String, dynamic>> _cachedReadings = ValueNotifier({});
  ValueNotifier<Map<String, dynamic>> get readings => _cachedReadings;

  void updateReadings(Map<String, dynamic> newReadings) {
    if (!mapEquals(_cachedReadings.value, newReadings)) {
      _cachedReadings.value = newReadings;
    }
  }

  String getFormattedValue(String key, {int decimals = 2}) {
    final value = _cachedReadings.value[key];
    if (value == null) return '0';
    return value.toStringAsFixed(decimals);
  }
}
