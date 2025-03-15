import 'package:flutter/material.dart';

class LazyMetricList extends StatelessWidget {
  final List<Widget> children;
  final ScrollController? controller;

  const LazyMetricList({super.key, required this.children, this.controller});

  @override
  Widget build(BuildContext context) {
    return ListView.builder(
      controller: controller,
      shrinkWrap: true,
      itemCount: children.length,
      itemBuilder: (context, index) {
        return RepaintBoundary(child: children[index]);
      },
    );
  }
}
