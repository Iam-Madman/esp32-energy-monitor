import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:firebase_auth/firebase_auth.dart';
import '../widgets/metric_cards.dart';
import '../widgets/device_status.dart';
import '../utils/dashboard_utils.dart';
import 'about_screen.dart';
import 'dart:async';
import '../services/metrics_service.dart';

class DashboardScreen extends StatefulWidget {
  const DashboardScreen({super.key});

  @override
  State<DashboardScreen> createState() => _DashboardScreenState();
}

class _DashboardScreenState extends State<DashboardScreen>
    with SingleTickerProviderStateMixin {
  final _database = FirebaseDatabase.instance.ref();
  final _metricsService = MetricsService();
  late AnimationController _animationController;
  Timer? _debounceTimer;
  StreamSubscription? _readingsSubscription;

  @override
  void initState() {
    super.initState();
    _animationController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 1000),
    );
    _animationController.forward();
    _setupDataSubscription();
  }

  void _setupDataSubscription() {
    _readingsSubscription = _database
        .child('readings')
        .onValue
        .transform(
          StreamTransformer.fromHandlers(
            handleData: (data, sink) {
              if (!mounted) return;
              _debounceTimer?.cancel();
              _debounceTimer = Timer(
                const Duration(milliseconds: 500),
                () => sink.add(data),
              );
            },
          ),
        )
        .listen((event) {
          if (!mounted) return;
          if (event is DatabaseEvent) {
            final data = event.snapshot.value as Map<dynamic, dynamic>?;
            if (data != null) {
              _metricsService.updateReadings(Map<String, dynamic>.from(data));
            }
          }
        });
  }

  @override
  void dispose() {
    _debounceTimer?.cancel();
    _readingsSubscription?.cancel();
    _animationController.dispose();
    super.dispose();
  }

  Future<void> _resetDevice() async {
    await _database.child('reset').set(true);
  }

  Future<void> _handleLogout() async {
    try {
      // First cancel all subscriptions and dispose resources
      _debounceTimer?.cancel();
      _readingsSubscription?.cancel();
      _animationController.dispose();

      // Then sign out
      await FirebaseAuth.instance.signOut();

      if (!mounted) return;

      // Navigate and remove all previous routes
      Navigator.of(context).pushNamedAndRemoveUntil('/login', (route) => false);
    } catch (e) {
      if (!mounted) return;
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Error logging out. Please try again.')),
      );
    }
  }

  Widget _buildAnimatedMetricCard(
    String title,
    dynamic value,
    String unit,
    IconData icon,
    int index,
  ) {
    final animation = CurvedAnimation(
      parent: _animationController,
      curve: Interval(
        (index * 0.1).clamp(0.0, 1.0),
        ((index + 1) * 0.1).clamp(0.0, 1.0),
        curve: Curves.easeOut,
      ),
    );

    return SlideTransition(
      position: Tween<Offset>(
        begin: const Offset(0.5, 0),
        end: Offset.zero,
      ).animate(animation),
      child: FadeTransition(
        opacity: animation,
        child: MetricCard(title: title, value: value, unit: unit, icon: icon),
      ),
    );
  }

  Widget _buildEnergyHighlightCard(dynamic value) {
    return Card(
      color: const Color(0xFF1A1A1A),
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
      elevation: 8,
      child: InkWell(
        onTap:
            () => showModalBottomSheet(
              context: context,
              backgroundColor: const Color(0xFF1A1A1A),
              builder: (context) => const MetricInfoSheet(metric: 'Energy'),
            ),
        child: Container(
          padding: const EdgeInsets.all(20),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  const Text(
                    'Total Energy Consumed',
                    style: TextStyle(
                      color: Color(0xFF5DB996),
                      fontSize: 18,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                  Icon(
                    Icons.energy_savings_leaf,
                    color: Theme.of(context).colorScheme.primary,
                    size: 24,
                  ),
                ],
              ),
              Row(
                crossAxisAlignment: CrossAxisAlignment.end,
                children: [
                  Text(
                    '${value?.toStringAsFixed(2) ?? "0.00"}',
                    style: const TextStyle(
                      color: Color(0xFFE3F0AF),
                      fontSize: 36,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                  const SizedBox(width: 8),
                  const Padding(
                    padding: EdgeInsets.only(bottom: 6),
                    child: Text(
                      'Wh',
                      style: TextStyle(color: Colors.white70, fontSize: 16),
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 8),
              Row(
                children: [
                  Icon(
                    Icons.info_outline,
                    color: Theme.of(context).colorScheme.primary,
                    size: 14,
                  ),
                  const SizedBox(width: 4),
                  Flexible(
                    child: Text(
                      'Tap any metric card to view detailed information',
                      style: TextStyle(
                        color: Theme.of(context).colorScheme.primary,
                        fontSize: 12,
                        fontStyle: FontStyle.italic,
                      ),
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 8),
              SizedBox(
                width: double.infinity,
                child: Row(
                  mainAxisSize: MainAxisSize.min,
                  children: [
                    Icon(
                      Icons.refresh,
                      color: Theme.of(context).colorScheme.primary,
                      size: 14,
                    ),
                    const SizedBox(width: 4),
                    Flexible(
                      child: Text(
                        'Use reset button above to clear energy counter',
                        style: TextStyle(
                          color: Theme.of(context).colorScheme.primary,
                          fontSize: 12,
                          fontStyle: FontStyle.italic,
                        ),
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildMetrics() {
    return ValueListenableBuilder<Map<String, dynamic>>(
      valueListenable: _metricsService.readings,
      builder: (context, readings, _) {
        return Column(
          children: [
            _buildEnergyHighlightCard(readings['energy']),
            const SizedBox(height: 24),
            _basicMetricsHeader,
            const SizedBox(height: 8),
            RepaintBoundary(
              child: Column(
                children: [
                  _buildAnimatedMetricCard(
                    'Voltage',
                    readings['voltage']?.toStringAsFixed(2),
                    'V',
                    Icons.flash_on,
                    0,
                  ),
                  _buildAnimatedMetricCard(
                    'Current',
                    readings['current']?.toStringAsFixed(2),
                    'A',
                    Icons.electrical_services, // Updated icon
                    1,
                  ),
                  _buildAnimatedMetricCard(
                    'Power',
                    readings['power']?.toStringAsFixed(2),
                    'W',
                    Icons.power,
                    2,
                  ),
                  _buildAnimatedMetricCard(
                    'Frequency',
                    readings['frequency']?.toStringAsFixed(2),
                    'Hz',
                    Icons.waves,
                    4,
                  ),
                ],
              ),
            ),
            const SizedBox(height: 16),
            const Text(
              'Advanced Metrics',
              style: TextStyle(
                color: Color(0xFF5DB996),
                fontSize: 20,
                fontWeight: FontWeight.bold,
              ),
            ),
            const SizedBox(height: 8),
            RepaintBoundary(
              child: Column(
                children: [
                  _buildAnimatedMetricCard(
                    'Apparent Power',
                    readings['apparentPower']?.toStringAsFixed(2),
                    'VA',
                    Icons.electric_meter, // Updated icon
                    5,
                  ),
                  _buildAnimatedMetricCard(
                    'Power Factor',
                    readings['powerFactor']?.toStringAsFixed(2),
                    '',
                    Icons.speed, // Updated icon
                    6,
                  ),
                  _buildAnimatedMetricCard(
                    'Reactive Power',
                    readings['reactivePower']?.toStringAsFixed(2),
                    'VAR',
                    Icons.sync, // Updated icon
                    7,
                  ),
                  _buildAnimatedMetricCard(
                    'Load Impedance',
                    readings['loadImpedance']?.toStringAsFixed(2),
                    'Ω',
                    Icons.settings_input_component, // Updated icon
                    8,
                  ),
                  _buildAnimatedMetricCard(
                    'THD',
                    readings['thd']?.toStringAsFixed(1),
                    '%',
                    Icons.multiline_chart, // Updated icon
                    9,
                  ),
                  _buildAnimatedMetricCard(
                    'Distortion Power',
                    readings['distortionPower']?.toStringAsFixed(2),
                    'VA',
                    Icons.show_chart, // Updated icon
                    10,
                  ),
                  _buildAnimatedMetricCard(
                    'Power Quality',
                    readings['powerQuality']?.toStringAsFixed(2),
                    '',
                    Icons.high_quality, // Updated icon
                    11,
                  ),
                ],
              ),
            ),
            if (readings['timestamp'] != null)
              Padding(
                padding: const EdgeInsets.only(top: 16),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Icon(
                      Icons.access_time,
                      size: 14,
                      color: Theme.of(
                        context,
                      ).colorScheme.primary.withAlpha(179),
                    ),
                    const SizedBox(width: 8),
                    Text(
                      'Last Updated: ${DashboardUtils.formatTimestamp(readings['timestamp'])}',
                      style: TextStyle(
                        color: Theme.of(
                          context,
                        ).colorScheme.secondary.withAlpha(179),
                        fontSize: 12,
                        fontStyle: FontStyle.italic,
                      ),
                    ),
                  ],
                ),
              ),
            const SizedBox(height: 16), // Add padding at the bottom
          ],
        );
      },
    );
  }

  static final _basicMetricsHeader = const Text(
    'Basic Metrics',
    style: TextStyle(
      color: Color(0xFF5DB996),
      fontSize: 20,
      fontWeight: FontWeight.bold,
    ),
  );

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFF121212),
      appBar: AppBar(
        backgroundColor: const Color(0xFF1A1A1A),
        elevation: 0,
        leading: Builder(
          builder:
              (context) => IconButton(
                icon: const Icon(Icons.menu),
                onPressed: () => Scaffold.of(context).openDrawer(),
              ),
        ),
        title: const Text('Energy Monitor', style: TextStyle(fontSize: 16)),
        actions: [
          const Padding(
            padding: EdgeInsets.symmetric(horizontal: 8.0),
            child: DeviceStatus(),
          ),
          StreamBuilder<DatabaseEvent>(
            stream: _database.child('battery').onValue,
            builder: (context, AsyncSnapshot<DatabaseEvent> batterySnapshot) {
              if (!batterySnapshot.hasData) return const SizedBox();

              return StreamBuilder<DatabaseEvent>(
                stream: _database.child('readings/isCharging').onValue,
                builder: (
                  context,
                  AsyncSnapshot<DatabaseEvent> chargingSnapshot,
                ) {
                  final battery =
                      (batterySnapshot.data!.snapshot.value as num?) ?? 0;
                  final batteryInt = battery.toInt();
                  final isCharging =
                      (chargingSnapshot.data?.snapshot.value as bool?) ?? false;

                  WidgetsBinding.instance.addPostFrameCallback((_) {
                    DashboardUtils.showBatteryWarning(context, batteryInt);
                  });

                  return Row(
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      Text(
                        '$batteryInt%',
                        style: TextStyle(
                          color:
                              batteryInt > 20
                                  ? Theme.of(context).colorScheme.primary
                                  : Theme.of(context).colorScheme.error,
                          fontWeight: FontWeight.bold,
                        ),
                      ),
                      const SizedBox(width: 4),
                      Icon(
                        isCharging
                            ? Icons.battery_charging_full
                            : DashboardUtils.getBatteryIcon(batteryInt),
                        color:
                            batteryInt > 20
                                ? Theme.of(context).colorScheme.primary
                                : Theme.of(context).colorScheme.error,
                      ),
                    ],
                  );
                },
              );
            },
          ),
          IconButton(
            icon: Icon(
              Icons.refresh,
              color: Theme.of(context).colorScheme.primary,
            ),
            onPressed: _resetDevice,
          ),
        ],
      ),
      drawer: Drawer(
        child: Container(
          color: const Color(0xFF1A1A1A),
          child: ListView(
            padding: EdgeInsets.zero,
            children: [
              DrawerHeader(
                decoration: const BoxDecoration(color: Color(0xFF5DB996)),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  mainAxisAlignment: MainAxisAlignment.end,
                  children: const [
                    Text(
                      'Energy Monitor',
                      style: TextStyle(color: Colors.white, fontSize: 24),
                    ),
                  ],
                ),
              ),
              ListTile(
                leading: const Icon(Icons.info_outline),
                title: const Text('About'),
                onTap: () {
                  Navigator.pop(context);
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (context) => const AboutScreen(),
                    ),
                  );
                },
              ),
              ListTile(
                leading: const Icon(Icons.logout),
                title: const Text('Logout'),
                onTap: () {
                  Navigator.pop(context);
                  _handleLogout();
                },
              ),
            ],
          ),
        ),
      ),
      body: ListView(
        padding: const EdgeInsets.all(16),
        children: [_buildMetrics()],
      ),
    );
  }
}
