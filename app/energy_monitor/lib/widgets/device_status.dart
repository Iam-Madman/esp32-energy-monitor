import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'dart:async';

class DeviceStatus extends StatefulWidget {
  const DeviceStatus({super.key});

  @override
  State<DeviceStatus> createState() => _DeviceStatusState();
}

class _DeviceStatusState extends State<DeviceStatus> {
  Timer? _timer;
  int? _lastSeen;
  bool _isOnline = false;

  @override
  void initState() {
    super.initState();
    // Start periodic check every second
    _timer = Timer.periodic(const Duration(seconds: 1), (_) {
      if (_lastSeen != null) {
        final now = DateTime.now().millisecondsSinceEpoch ~/ 1000;
        final difference = now - _lastSeen!;
        final newStatus = difference <= 3;

        if (newStatus != _isOnline && mounted) {
          setState(() => _isOnline = newStatus);
        }

        debugPrint(
          'Last seen: $_lastSeen, Now: $now, Diff: $difference seconds, Online: $_isOnline',
        );
      }
    });
  }

  @override
  void dispose() {
    _timer?.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final databaseRef = FirebaseDatabase.instance.ref();

    return StreamBuilder<DatabaseEvent>(
      stream: databaseRef.child('deviceStatus/lastSeen').onValue,
      builder: (context, AsyncSnapshot<DatabaseEvent> snapshot) {
        if (!snapshot.hasData) {
          return const Icon(Icons.circle, size: 12, color: Colors.grey);
        }

        _lastSeen = snapshot.data!.snapshot.value as int?;
        if (_lastSeen == null) {
          return const Icon(Icons.circle, size: 12, color: Colors.grey);
        }

        return Icon(
          Icons.circle,
          size: 12,
          color: _isOnline ? Colors.green : Colors.red,
        );
      },
    );
  }
}
